// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "parse.h"

#include <stdint.h>

#include "uwuify.h"

char uwu_uwuify_char(char input) {
	switch (input) {
		case 'r':
		case 'l':
			return 'w';
			break;
		case 'R':
		case 'L':
			return 'W';
			break;
		default:
			return input;
			break;
	}
}

static uint64_t rng_next(uint64_t *state) {
	uint64_t x = *state;

	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;

	*state = x;

	return x * UINT64_C(2685821657736338717);
}

static bool do_stutter(uwuify_instance *instance) {
	switch (instance->stutter_chance) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return ((rng_next(&instance->rng) %
			         instance->stutter_chance) == 0);
	}
}
