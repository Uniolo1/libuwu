// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "uwuify.h"

#include <stdint.h>
#include <time.h>

#include "dictionary.h"
#include "parse.h"

const char *uwu_INFO = "libuwuify 0.0.0";
const uint16_t uwu_VERSION[3] = {0, 0, 0};

static const uint8_t
    number_of_defaults; // used for determining default dictionary size

uint8_t uwu_init(uwuify_instance *instance) {
	if (instance->initalized) uwu_close(instance);

	instance->replacement_dictionary =
	    uwu_dict_create(number_of_defaults + 5);

	if (instance->replacement_dictionary == NULL) return 1;

	instance->stutter_chance = DEFAULT_STUTTER_CHANCE;
	instance->rng = (uint64_t)time(NULL);

	instance->initalized = true;
	return 0;
}

void uwu_close(uwuify_instance *instance) {
	uwu_dict_free(instance->replacement_dictionary);
	instance->replacement_dictionary = NULL;

	instance->initalized = false;
}

char *uwu_uwuify(uwuify_instance *instance, char *input) {
	return uwu_uwuify_text(instance, input);
}

void uwu_update_stutter_chance(uwuify_instance *instance, uint8_t new_chance) {
	instance->stutter_chance = new_chance;
}

char *uwu_replacement_get_value(uwuify_instance *instance, const char *key) {
	return uwu_dict_get(instance->replacement_dictionary, key);
}

uint8_t uwu_replacement_update(uwuify_instance *instance, const char *key,
                               const char *value) {
	return uwu_dict_set(instance->replacement_dictionary, key, value);
}

uint8_t uwu_replacement_remove(uwuify_instance *instance, const char *key) {
	uwu_dict_set(instance->replacement_dictionary, key, NULL);
	return 0;
}

static const uint8_t number_of_defaults = 7;
uint8_t uwu_replacement_load_defaults(uwuify_instance *instance) {
	if (uwu_dict_set(instance->replacement_dictionary, "love", "wuv"))
		return 1;
	if (uwu_dict_set(instance->replacement_dictionary, "loved", "wuved"))
		return 1;
	if (uwu_dict_set(instance->replacement_dictionary, "this", "dis"))
		return 1;
	if (uwu_dict_set(instance->replacement_dictionary, "small", "smol"))
		return 1;
	if (uwu_dict_set(instance->replacement_dictionary, "windows", "wuduws"))
		return 1;
	if (uwu_dict_set(instance->replacement_dictionary, "angry", "angi"))
		return 1;
	if (uwu_dict_set(instance->replacement_dictionary, "guh", "buh"))
		return 1;

	return 0;
}
