// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software,
 * either in source code form or as a compiled binary, for any purpose, commercial or
 * non-commercial, and by any means.

 * In jurisdictions that recognize copyright laws, the author or authors of this software
 * dedicate any and all copyright interest in the software to the public domain. We make this
 * dedication for the benefit of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of relinquishment in perpetuity of
 * all present and future rights to this software under copyright law.

 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
 * OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define TESTC_H_IMPLEMENTATION
#include "testc.h"

#include "../libuwu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declared globally
uwu_instance instance_uwu = {0};

int uwuify_Error(void)
{
	instance_uwu.stutter_chance = 0;
	char *out = uwu_uwuify(&instance_uwu, "Error");

	if (out != NULL)
		printf("\"%s\" == \"Errwu\"\n", out);

	int ret = strcmp(out, "Errwu");
	free(out);
	return ret;
}

int no_multibyte_stutter(void)
{
	instance_uwu.stutter_chance = 1;
	char *out = uwu_uwuify(&instance_uwu, "π");

	if (out != NULL)
		printf("\"%s\" == \"π\"\n", out);

	int ret = strcmp(out, "π");
	free(out);
	return ret;
}

static inline char get_rand_char(void)
{
	switch (rand() % 5)
	{
	case 0:
		return 'a';
	case 1:
		return 'b';
	case 2:
		return 'l';
	case 3:
		return 'w';
	case 4:
		return 'W';
	default:
		puts("Impossible statement reached!");
		exit(1);
	}
}

int large_test_9064(void)
{
	// this is more to test against segfaults
	instance_uwu.stutter_chance = 1;
	char *input = malloc(9064 + 1);
	input[9064] = '\0';

	for (int i = 0; i < 9064; i++)
		input[i] = get_rand_char();

	char *output = uwu_uwuify_mutonly(&instance_uwu, input);

	// ensure output is null-terminated
	for (int i = 0; output[i] != '\0'; i++)
	{
		// pass
	}

	free(output);
	free(input);
	return 0;
}

int get_items_in_replacement_dictionary(void)
{
	// this is more to test against segfaults

	// reinitalie:
	if (uwu_init(&instance_uwu))
	{
		uwu_perrwu(&instance_uwu, "Failed to initalize libuwu");
		return 1;
	}

	instance_uwu.errwu = "";

	uwu_replacement_update(&instance_uwu, "a", "b");
	uwu_replacement_update(&instance_uwu, "c", "d");
	uwu_replacement_update(&instance_uwu, "e", "f");

	if (instance_uwu.errwu[0] != '\0')
		return 2;

	// uwuify some test (instantly free output)
	free(uwu_uwuify(&instance_uwu, "A uwuify uwuifies, and this tests c, E, and b !"));

	char **output = NULL;
	long len = uwu_replacement_get_items(&instance_uwu, &output);
	if (len == -1 || output == NULL)
		return 3;

	int ret = 0;

	printf("'%ld' == '3'\n", len);
	if (len != 3)
	{
		ret = 4;
		goto exit_function;
	}

	puts("output[0] == 'a'");
	if (strcmp(output[0], "a") != 0)
		ret = 5;

	puts("output[1] == 'c'");
	if (strcmp(output[1], "c") != 0)
		ret = 6;

	puts("output[2] == 'e'");
	if (strcmp(output[2], "e") != 0)
		ret = 7;

	puts("output[3] == NULL");
	if (output[3] != NULL)
		ret = 8;

exit_function:
	uwu_free_result_of_replacement_get_items(&output);
	return ret;
}

int main(void)
{
	printf("%s\n", uwu_INFO);
	printf("%s\n", testc_INFO);

	testc_tests *instance_test = testc_get();
	if (testc_init(instance_test))
	{
		puts("Failed to initalize testc");
		return 1;
	}

	if (uwu_init(&instance_uwu))
	{
		uwu_perrwu(&instance_uwu, "Failed to initalize libuwu");
		return 1;
	}

	if (uwu_replacement_load_defaults(&instance_uwu))
	{
		uwu_perrwu(&instance_uwu, "Failed to load replacements");
		return 2;
	}

	testc_add_test(instance_test, uwuify_Error, "uwuify-Error");
	testc_add_test(instance_test, no_multibyte_stutter, "no-multi-byte-stutter");
	testc_add_test(instance_test, large_test_9064, "large-9064");
	// REINITALIZES instance_uwu DO LAST:
	testc_add_test(instance_test, get_items_in_replacement_dictionary,
	               "get-items-in-replacement-dictionary");

	long result = run_tests(instance_test);
	if (result != 0)
		result += 2;

	testc_free(instance_test);
	uwu_close(&instance_uwu);
	return (int)result;
}
