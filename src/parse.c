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

// default instance->internal->rng_function
// DO NOT CALL DIRECTLY, call instance->internal->rng_function instead!
// xorshift64* Marsaglia/Vigna-style variant
uint64_t uwu_rng_next(uint64_t *state)
{
	uint64_t x = *state;

	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;

	*state = x;

	return x * UINT64_C(2685821657736338717);
}

// stuttering logic is not made for multi-byte characters
static inline bool is_first_character_multibyte(const char *input)
{
	unsigned char c;
	c = (unsigned char)input[0];

	return (c & 0x80) != 0;
}

static inline bool do_stutter(uwu_instance *instance, char *string)
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
		return ((instance->internal->rng_function(&instance->rng) %
		         instance->stutter_chance) == 0);
	}
}

static inline char *match_and_replace_string(uwu_instance *instance, const char *input)
{
	char *ret = uwu_dict_get(instance->internal->replacement_dictionary, input);
	uwu_transfer_case(input, &ret);
	return ret;
}

static inline bool ensure_output_capacity(char **output, size_t *capacity, size_t length,
                                          size_t additional)
{
	size_t required;
	size_t new_capacity;
	char *tmp;

	if (additional > SIZE_MAX - length - 1)
		return false;

	required = length + additional + 1;

	if (required <= *capacity)
		return true;

	new_capacity = *capacity;

	if (new_capacity == 0)
		new_capacity = 1;

	while (new_capacity < required)
	{
		if (new_capacity > SIZE_MAX / 2)
		{
			new_capacity = required;
			break;
		}

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
	// create output string
	size_t output_len = 0;
	size_t output_cap = strlen(input) + 1;
	char *output = malloc(output_cap);

	if (output == NULL)
	{
		instance->errwu = "failed to allocate memory for output string";
		return NULL;
	}

	output[0] = '\0';

	bool add_space = false;
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
			size_t word_len = strlen(word);
			size_t additional;
			size_t i;

			if (stutter)
			{
				if (word_len > (SIZE_MAX / 2))
				{
					free(output);
					instance->errwu = "failed to resize output string";
					return NULL;
				}

				additional = word_len * 2;
			}
			else
			{
				additional = word_len;
			}

			if (!ensure_output_capacity(&output, &output_cap, output_len,
			                            additional))
			{
				free(output);
				instance->errwu = "failed to resize output string";
				return NULL;
			}

			for (i = 0; i < word_len; i++)
			{
				char character = uwuify_char(word[i]);

				output[output_len++] = character;

				if (stutter)
					output[output_len++] = '-';
			}

			output[output_len] = '\0';
		}
		else
		{
			size_t match_len = strlen(match);
			size_t additional;

			/*
			 * Reserve enough space for the replacement, if
			 * it is larger than the original input word.
			 */
			if (stutter)
			{
				if (match_len > SIZE_MAX - 2)
				{
					free(output);
					instance->errwu = "failed to resize output string";
					return NULL;
				}

				additional = match_len + 2;
			}
			else
			{
				additional = match_len;
			}

			if (!ensure_output_capacity(&output, &output_cap, output_len,
			                            additional))
			{
				free(output);
				instance->errwu = "failed to resize output string";
				return NULL;
			}

			if (stutter)
			{
				output[output_len++] = match[0];
				output[output_len++] = '-';
			}

			memcpy(output + output_len, match, match_len);
			output_len += match_len;
			output[output_len] = '\0';
		}

		word = strtok(NULL, " ");
	}

	return output;
}
