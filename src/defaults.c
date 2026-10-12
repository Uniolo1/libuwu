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

// the header file for defaults.c is shared with uwuify.c
#include "h/uwuify.h"

#include <stdint.h>

#include "h/dictionary.h"

static const char *default_replace[][2] = {
    {"love", "wuv"},        {"loved", "wuved"},   {"this", "dis"}, {"small", "smol"},
    {"windows", "wuduws"},  {"angry", "angi"},    {"guh", "bug"},  {"boy", "boi"},
    {"error", "errwu"},     {"errors", "errwus"}, {"hi", "hai"},   {":)", ":3"},
    {"library", "wibwary"}, {NULL, NULL},
};

// UWU_ARRAY_SIZE comes from an LLM (better then setting manually)
#define UWU_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
const uint8_t uwu_number_of_default_replacements = UWU_ARRAY_SIZE(default_replace);

uint8_t uwu_replacement_load_defaults(uwu_instance *instance)
{
	uint8_t ret = 0;

	for (int i = 0; default_replace[i][0] != NULL; i++)
	{
		if (uwu_dict_set(instance->internal->replacement_dictionary,
		                 default_replace[i][0], default_replace[i][1]))
			ret++;
	}

	if (ret != 0)
		instance->errwu = "failed to allocate memory for one ore more new item's";

	return ret;
}
