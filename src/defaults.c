// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

// the header file for defaults.c is shared with uwuify.c
#include "uwuify.h"

#include <stdint.h>

#include "dictionary.h"

const uint8_t uwu_number_of_default_replacements = 12;
static inline uint8_t private_replacement_load_defaults(uwu_instance *instance)
{
	uint8_t ret = 0;

	if (uwu_dict_set(instance->internal.replacement_dictionary, "love", "wuv"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "loved", "wuved"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "this", "dis"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "small", "smol"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "windows", "wuduws"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "angry", "angi"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "guh", "buh"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "boy", "boi"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "error", "errwu"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "errors", "errwus"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, "hi", "hai"))
		ret++;
	if (uwu_dict_set(instance->internal.replacement_dictionary, ":)", ":3"))
		ret++;

	return ret;
}

// wrapper around private_replacement_load_defaults that sets errwu
uint8_t uwu_replacement_load_defaults(uwu_instance *instance)
{
	uint8_t ret = private_replacement_load_defaults(instance);
	if (ret != 0)
		instance->errwu = "failed to allocate memory for one ore more new item's";

	return ret;
}
