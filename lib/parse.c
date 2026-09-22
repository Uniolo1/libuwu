// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "parse.h"

#include <stdint.h>
#include <string.h>

#include "dictionary.h"
#include "uwuify.h"

static char uwuify_char(char input) {
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

static bool is_first_character_multibyte(const char *input) {
	unsigned char c;
	c = (unsigned char)input[0];

	return (c & 0x80) != 0;
}

static bool do_stutter(uwuify_instance *instance, char *string) {
	if (is_first_character_multibyte(string)) return false;

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

static char *match_and_replace_string(uwuify_instance *instance,
                                      const char *input) {
	return uwu_dict_get(instance->replacement_dictionary, input);
}

char *uwu_uwuify_text(uwuify_instance *instance, char *input) {
	int output_len = strlen(input) + 1;
	char *output = malloc(output_len);

	if (output == NULL) return NULL;

	// ensure output is always NULL-Terminated
	output[0] = '\0';

	bool add_space = false;

	char *word = strtok(input, " ");
	while (word != NULL) {
		// logic for adding space between words
		if (add_space) {
			size_t output_len = strlen(output);

			// append the space to 'output'
			output[output_len] = ' ';
			output[output_len + 1] = '\0';
		} else
			add_space = true;

		char *match = match_and_replace_string(instance, word);

		bool stutter = do_stutter(instance, word);

		if (match == NULL) {
			for (int i = 0; word[i] != '\0'; i++) {
				size_t output_len = strlen(output);

				// append the character to 'output'
				char character = uwuify_char(word[i]);

				if (stutter) {
					char *tmp =
					    realloc(output, output_len + 3);
					if (tmp != NULL) {
						output = tmp;

						output[output_len] = character;
						output[output_len] = '-';
						output[output_len] = '\0';
						output_len = output_len + 3;
					} else {
						/* allocation failed; output
						 * remains unchanged */
					}
				}

				output[output_len] = character;
				output[output_len + 1] = '\0';
			}
		} else

		    if (stutter) {
			char *tmp = realloc(output, output_len + 3);
			if (tmp != NULL) {
				output = tmp;

				output[output_len] = match[0];
				output[output_len] = '-';
				output[output_len] = '\0';
				output_len = output_len + 3;
			} else {
				/* allocation failed; output
				 * remains unchanged */
			}
		}

		// append the match
		strcat(output, match);

		// move onto next word
		word = strtok(NULL, " ");
	}
	return output;
}
