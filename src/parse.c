// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "parse.h"

#include <stdint.h>
#include <string.h>

#include "case.h"
#include "dictionary.h"
#include "uwuify.h"

// replace r and l with w
static char uwuify_char(char input)
{
	switch (input)
	{
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

// xorshift64* Marsaglia/Vigna-style variant
static inline uint64_t rng_next(uint64_t *state)
{
	uint64_t x = *state;

	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;

	*state = x;

	return x * UINT64_C(2685821657736338717);
}

static inline bool is_first_character_multibyte(const char *input)
{
	unsigned char c;
	c = (unsigned char)input[0];

	return (c & 0x80) != 0;
}

static bool do_stutter(uwu_instance *instance, char *string)
{
	if (is_first_character_multibyte(string))
		return false;

	switch (instance->stutter_chance)
	{
	case 0:
		return false;
	case 1:
		return true;
	default:
		return ((rng_next(&instance->rng) % instance->stutter_chance) == 0);
	}
}

// TODO: Make matching somewhat case-insensitive (preserve case to the greatest
// extent possible)
static inline char *match_and_replace_string(uwu_instance *instance, const char *input)
{
	// With this the output (if found) will always be lowercase:
	/*
	        char *ret = NULL;
	        char *lower_input = uwu_strdup(input);

	        if (lower_input != NULL)
	        {
	                uwu_make_lowercase(&lower_input);
	                ret = uwu_dict_get(instance->replacement_dictionary, lower_input);
	                free(lower_input);
	        }
	        else
	        {
	                // skip the lowercase step
	                ret = uwu_dict_get(instance->replacement_dictionary, input);
	        }

	        uwu_transfer_case(input, &ret);
	        return ret;
	 */
	// This perserves case but a match is only found if the input is all lowercase:
	char *ret = uwu_dict_get(instance->replacement_dictionary, input);
	uwu_transfer_case(input, &ret);
	return ret;
}

// WARNING: AI used heavily here
static inline bool ensure_output_capacity(char **output, size_t *capacity, size_t length,
                                          size_t additional)
{
	size_t required;
	size_t new_capacity;
	char *tmp;

	required = length + additional + 1;

	if (required <= *capacity)
		return true;

	new_capacity = *capacity;

	while (new_capacity < required)
	{
		new_capacity *= 2;
	}

	tmp = realloc(*output, new_capacity);
	if (tmp == NULL)
		return false;

	*output = tmp;
	*capacity = new_capacity;

	return true;
}

// WARNING: AI used heavily here (primarily in the string resizing logic)
char *uwu_uwuify_text(uwu_instance *instance, char *input)
{
	size_t output_len = 0;
	size_t output_cap = strlen(input) + 1;
	char *output = malloc(output_cap);

	bool add_space = false;

	if (output == NULL)
	{
		instance->errwu = "failed to allocate memory for output string";
		return NULL;
	}

	output[0] = '\0';

	char *word = strtok(input, " ");

	while (word != NULL)
	{
		if (add_space)
		{
			if (!ensure_output_capacity(&output, &output_cap, output_len, 1))
			{
				free(output);
				instance->errwu = "failed to resize output string";
				return NULL;
			}

			output[output_len++] = ' ';
			output[output_len] = '\0';
		}
		else
		{
			add_space = true;
		}

		char *match = match_and_replace_string(instance, word);
		bool stutter = do_stutter(instance, word);

		if (match == NULL)
		{
			size_t i;

			for (i = 0; word[i] != '\0'; i++)
			{
				char character = uwuify_char(word[i]);

				if (stutter)
				{
					if (!ensure_output_capacity(&output, &output_cap,
					                            output_len, 2))
					{
						free(output);
						instance->errwu = "failed to resize output "
						                  "string";
						return NULL;
					}

					output[output_len++] = character;
					output[output_len++] = '-';
					output[output_len] = '\0';
				}
				else
				{
					if (!ensure_output_capacity(&output, &output_cap,
					                            output_len, 1))
					{
						free(output);
						instance->errwu = "failed to resize output "
						                  "string";
						return NULL;
					}

					output[output_len++] = character;
					output[output_len] = '\0';
				}
			}
		}
		else
		{
			size_t match_len = strlen(match);

			if (stutter)
			{
				if (!ensure_output_capacity(&output, &output_cap, output_len,
				                            2))
				{
					free(output);
					instance->errwu = "failed to resize output string";
					return NULL;
				}

				output[output_len++] = match[0];
				output[output_len++] = '-';
				output[output_len] = '\0';
			}

			if (!ensure_output_capacity(&output, &output_cap, output_len,
			                            match_len))
			{
				free(output);
				instance->errwu = "failed to resize output string";
				return NULL;
			}

			memcpy(output + output_len, match, match_len);
			output_len += match_len;
			output[output_len] = '\0';
		}

		word = strtok(NULL, " ");
	}

	return output;
}
