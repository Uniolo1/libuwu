// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

// the header file for defaults.c is shared with uwuify.c
#include "uwuify.h"

#include <stdint.h>

#include "dictionary.h"

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
