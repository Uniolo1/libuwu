// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "uwuify.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "dictionary.h"
#include "parse.h"

const char *uwu_INFO = "libuwu v1.0.2";
const uint16_t uwu_VERSION[3] = {1, 0, 2};
// 'uwu_number_of_default_replacements' defined in defaults.c
#define DEFAULT_STUTTER_CHANCE 16 // 1 in 16

uint8_t uwu_init(uwu_instance *instance)
{
	if (instance->internal.initalized)
		// close and then reinitalize
		uwu_close(instance);

	instance->internal.replacement_dictionary =
	    uwu_dict_create(uwu_number_of_default_replacements + 1);

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

/*
 * implementation of:
 * 	uint8_t uwu_replacement_load_defaults(uwu_instance *instance);
 * has been moved to defaults.c
 */
