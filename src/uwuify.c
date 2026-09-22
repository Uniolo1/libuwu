// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "uwuify.h"

#include <stdint.h>
#include <time.h>

#include "dictionary.h"

const char *uwu_INFO = "libuwuify 0.0.0";
const uint16_t version[3] = {0, 0, 0};

void uwu_init(uwuify_instance *instance) {
	if (instance->initalized) uwu_close(instance);

	instance->stutter_chance = DEFAULT_STUTTER_CHANCE;
	instance->rng = (uint64_t)time(NULL);

	instance->initalized = true;
}

void uwu_close(uwuify_instance *instance) {
	dict_free(instance->replacement_dictionary);
	instance->replacement_dictionary = NULL;

	instance->initalized = false;
}

static uint64_t rng_next(uint64_t *state) {
	uint64_t x = *state;

	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;

	*state = x;

	return x * UINT64_C(2685821657736338717);
}

char *uwu_replacement_get_value(uwuify_instance *instance, const char *key) {
	return dict_get(instance->replacement_dictionary, key);
}

uint8_t uwu_replacement_update(uwuify_instance *instance, const char *key,
                               const char *value) {
	return dict_set(instance->replacement_dictionary, key, value);
}

uint8_t uwu_replacement_remove(uwuify_instance *instance, const char *key) {
	dict_set(instance->replacement_dictionary, key, NULL);
	return 0;
}

uint8_t uwu_replacement_load_defaults(uwuify_instance *instance) {
	if (dict_set(instance->replacement_dictionary, "love", "wuv")) return 1;
	if (dict_set(instance->replacement_dictionary, "loved", "wuved"))
		return 1;
	if (dict_set(instance->replacement_dictionary, "this", "dis")) return 1;
	if (dict_set(instance->replacement_dictionary, "small", "smol"))
		return 1;
	if (dict_set(instance->replacement_dictionary, "windows", "wuduws"))
		return 1;
	if (dict_set(instance->replacement_dictionary, "angry", "angi"))
		return 1;
	if (dict_set(instance->replacement_dictionary, "guh", "buh")) return 1;

	return 0;
}
