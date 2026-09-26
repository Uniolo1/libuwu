#define TESTC_H_IMPLEMENTATION
#include "testc.h"

// demo programs use relative paths so clangd can shutup
#include "../libuwu.h"

#include <stdio.h>
#include <string.h>

// declared globally
uwu_instance instance_uwu;

int uwuify_Error(void)
{
	instance_uwu.stutter_chance = 0;
	char *out = uwu_uwuify(&instance_uwu, "Error");

	if (out != NULL)
		printf("\"%s\" == \"Errwu\"\n", out);

	return strcmp(out, "Errwu");
}

int no_multibyte_stutter(void)
{
	instance_uwu.stutter_chance = 1;
	char *out = uwu_uwuify(&instance_uwu, "π");

	if (out != NULL)
		printf("\"%s\" == \"π\"\n", out);

	return strcmp(out, "π");
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

	return (int)run_tests(instance_test);
}
