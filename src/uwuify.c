// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "uwuify.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "dictionary.h"
#include "parse.h"

const char *uwu_INFO = "libuwu v0.0.0";
const uint16_t uwu_VERSION[3] = {0, 0, 0};
#define DEFAULT_STUTTER_CHANCE 24 // 1 in 12

static const uint8_t number_of_defaults; // used for determining default dictionary size

uint8_t uwu_init(uwu_instance *instance)
{
	if (instance->internal.initalized)
		// close and then reinitalize
		uwu_close(instance);

	instance->internal.replacement_dictionary = uwu_dict_create(number_of_defaults + 1);

	if (instance->internal.replacement_dictionary == NULL)
	{
		instance->errwu = "failed to allocate memory for dictionary";
		return 1;
	}

	instance->stutter_chance = DEFAULT_STUTTER_CHANCE;
	instance->rng = 0;
	instance->errwu = NULL;

	instance->internal.initalized = true;
	return 0;
}

// WARNING: any instance that is closed must be re-initalized!
void uwu_close(uwu_instance *instance)
{
	uwu_dict_free(instance->internal.replacement_dictionary);
	instance->internal.replacement_dictionary = NULL;

	instance->internal.initalized = false;
}

char *uwu_uwuify(uwu_instance *instance, char *input)
{
	// NOTE: errwu set by uwu_uwuify_text
	return uwu_uwuify_text(instance, input);
}

void uwu_perrwu(uwu_instance *instance, char *messsage)
{
	// basically just perror but libuwu, ensures errwu is not NULL.
	printf("%s:", messsage);
	if (instance->errwu != NULL)
		printf(" %s", instance->errwu);
	printf("\n");
}

void uwu_clear_errwu(uwu_instance *instance)
{
	instance->errwu = NULL;
}

char *uwu_replacement_get_value(uwu_instance *instance, const char *key)
{
	return uwu_dict_get(instance->internal.replacement_dictionary, key);
}

uint8_t uwu_replacement_update(uwu_instance *instance, const char *key, const char *value)
{
	uint8_t ret = uwu_dict_set(instance->internal.replacement_dictionary, key, value);

	if (ret != 0)
		instance->errwu = "failed to allocate memory for new item";

	return ret;
}

uint8_t uwu_replacement_remove(uwu_instance *instance, const char *key)
{
	uwu_dict_set(instance->internal.replacement_dictionary, key, NULL);
	return 0;
}

static const uint8_t number_of_defaults = 11;
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
	if (uwu_dict_set(instance->internal.replacement_dictionary, "hi", "hii"))
		ret++;

	return ret;
}

// wrapper around private_replacement_load_defaults that sets errwu
uint8_t uwu_replacement_load_defaults(uwu_instance *instance)
{
	uint8_t ret = private_replacement_load_defaults(instance);
	if (ret != 0)
		instance->errwu = "failed to allocate memory for a new item";
	return ret;
}
