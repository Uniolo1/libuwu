// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "parse.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *uwuify_string(char *input) {
	char *output = malloc(strlen(input) + 1);
	if (output == NULL) {
		perror("malloc");
		return NULL;
	}

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

		char *match = replace_string(word);

		if (match == NULL) {
			for (int i = 0; word[i] != '\0'; i++) {
				size_t output_len = strlen(output);

				// append the character to 'output'
				output[output_len] = replace_char(word[i]);
				output[output_len + 1] = '\0';
			}
		} else
			// append the match
			strcat(output, match);

		// move onto next word
		word = strtok(NULL, " ");
	}
	return output;
}

// NOTE: AI was used here after struggling for ~30 minutes to fix an issue!
// Said issue was punctuation like periods breaking the replacement algorithm.
char *replace_string(const char *input) {
	static char *output = NULL;

	size_t len = strlen(input);
	size_t word_len = len;

	if (len > 0 && (input[len - 1] == '.' || input[len - 1] == ',' ||
	                input[len - 1] == '?' || input[len - 1] == '!')) {
		word_len--;
	}

	const char *replacement = NULL;

	// WARNING: the length of the replacement must be <= that of the input
	if (word_len == 4 && strncmp(input, "love", word_len) == 0)
		replacement = "wuv";
	if (word_len == 5 && strncmp(input, "loved", word_len) == 0)
		replacement = "wuved";
	else if (word_len == 5 && strncmp(input, "small", word_len) == 0)
		replacement = "smol";
	else if (word_len == 4 && strncmp(input, "this", word_len) == 0)
		replacement = "dis";
	else if (word_len == 7 && strncmp(input, "windows", word_len) == 0)
		replacement = "wuduws";
	else if (word_len == 3 && strncmp(input, "boy", word_len) == 0)
		replacement = "boi";
	else if (word_len == 5 && strncmp(input, "angry", word_len) == 0)
		replacement = "angi";
	else if (word_len == 3 && strncmp(input, "guh", word_len) == 0)
		replacement = "buh";
	else if (word_len == 2 && strncmp(input, ":)", word_len) == 0)
		replacement = ":3";

	if (!replacement) return NULL;

	size_t replacement_len = strlen(replacement);

	free(output);
	output = malloc(replacement_len + (len - word_len) + 1);

	if (!output) return NULL;

	memcpy(output, replacement, replacement_len);
	memcpy(output + replacement_len, input + word_len, len - word_len);
	output[replacement_len + (len - word_len)] = '\0';

	return output;
}

char replace_char(char input) {
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
