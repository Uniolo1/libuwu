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

// FIXME: Punctuation breaks this
char *replace_string(char *input) {
	// WARNING: Only return strings with a length <= that of the input
	if (strcmp(input, "love") == 0) return "wuv";
	if (strcmp(input, "small") == 0) return "smol";
	if (strcmp(input, "this") == 0) return "dis";
	if (strcmp(input, "windows") == 0) return "wuduws";
	if (strcmp(input, "boy") == 0) return "boi";
	if (strcmp(input, "angry") == 0) return "angi";
	return NULL;
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
