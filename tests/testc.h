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
 *
 */

#ifndef TESTC_H
#define TESTC_H
#include <stdlib.h>

extern const char *testc_INFO;
extern const unsigned testc_VERSION[3];

struct testcs_test;
typedef struct testc_tests testc_tests;

// initalization
testc_tests *testc_get(void);
int testc_init(testc_tests *instance);
void testc_free(testc_tests *instance);

// add tests
int testc_add_test(testc_tests *instance, int (*function)(void),
                   const char *restrict name); // BEWARE: if 'name' goes out scope, undefined
                                               // behavior will ensue!

long run_tests(testc_tests *instance);

#ifdef TESTC_H_IMPLEMENTATION

#include <stdbool.h>
#include <stdio.h>

typedef struct testcs_test
{
	int (*function)(void);
	const char *name;
} testcs_test;

struct testc_tests
{
	bool initalized;

	size_t count;
	size_t cap;

	testcs_test *tests;
};

const char *testc_INFO = "testc v1.0.1 <https://github.com/uniolo1/testc>";
const unsigned testc_VERSION[3] = {1, 0, 1};

testc_tests *testc_get(void)
{
	testc_tests *ret = malloc(sizeof(testc_tests));
	if (ret == NULL)
		return NULL;

	ret->tests = NULL;
	ret->count = 0;
	ret->initalized = false;
	ret->cap = 10;

	return ret;
}

int testc_init(testc_tests *instance)
{
	if (instance == NULL)
		return 1;

	if (instance->initalized)
	{
		// reinitalize
		testc_free(instance);
		instance = testc_get();
	}

	instance->tests = calloc(instance->cap, sizeof(*instance->tests));
	if (instance->tests == NULL)
		return 2;

	instance->initalized = true;

	return 0;
}

void testc_free(testc_tests *instance)
{
	free(instance->tests);
	free(instance);
}

int testc_add_test(testc_tests *instance, int (*function)(void), const char *name)
{
	if (name == NULL || function == NULL)
		return 2;

	if (instance->count == instance->cap)
	{
		size_t new_cap = instance->cap * 2;

		void *temp_ptr = realloc(instance->tests, new_cap * sizeof(*instance->tests));

		if (temp_ptr == NULL)
			return 1;

		instance->tests = temp_ptr;
		instance->cap = new_cap;
	}

	instance->tests[instance->count].function = function;
	instance->tests[instance->count].name = name;
	instance->count++;

	return 0;
}

long run_tests(testc_tests *instance)
{
	if (instance->count == 0)
	{
		puts("No tests to run!");
		return -1;
	}

	printf("Running %zu tests\n", instance->count);
	long failed = 0;

	for (size_t i = 0; i < instance->count; i++)
	{
		printf("\n--- \"%s\" (%zu) ---\n", instance->tests[i].name, i);
		int result = instance->tests[i].function();

		if (result != 0)
		{
			printf("[%d] FAIL: \"%s\" (%zu)\n", result, instance->tests[i].name,
			       i);
			failed++;
		}
		else
			printf("[0] PASS: \"%s\" (%zu)\n", instance->tests[i].name, i);
	}

	printf("\nSummary: %zu/%zu tests passed", (instance->count - failed), instance->count);
	return failed;
}

#endif

#endif
