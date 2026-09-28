// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

// demo programs use relative paths so clangd can shutup
#include "../libuwu.h"

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEFAULT_STUTTER_CHANCE 6 // 1 in 6
#define DEFAULT_INPUT_INDEX -1

static bool read_stdin = false;
static int input_index = DEFAULT_INPUT_INDEX;

// *minor* AI usage here (bug-fixing)
static inline char *write_entire_stdin_to_string(void)
{
	size_t capacity = 1024;
	size_t length = 0;

	char *output = malloc(capacity);
	if (output == NULL)
		return NULL;

	int c;

	while ((c = fgetc(stdin)) != EOF)
	{
		if (length + 1 >= capacity)
		{
			capacity *= 2;

			char *tmp = realloc(output, capacity);
			if (tmp == NULL)
			{
				free(output);
				return NULL;
			}

			output = tmp;
		}

		output[length++] = (char)c;
	}

	output[length] = '\0';
	return output;
}

static inline char *get_argument(char *restrict arg, const char *restrict name)
{
	size_t len = strlen(name);

	if (strncmp(arg, name, len) == 0)
		return arg + len;

	return NULL;
}

static void print_help(const char *program_name)
{
	printf("%s\n", uwu_INFO);

	printf("Usage: %s <input> [options]\n", program_name);
	printf("\n");
	printf("Options:\n");
	printf("  --stutter-chance=<0-256>  Stutter chance (default: %d)\n",
	       DEFAULT_STUTTER_CHANCE);
	printf("  --rng-seed=<0+>           RNG seed (default: UNIX time)\n");
	printf("  --stdin                   Read from stdin instead of argv[1]");
}

static inline void parse_arguments(char *argv[], uwu_instance *instance)
{
	for (int i = 1; argv[i] != NULL; i++)
	{
		char *flag = NULL;

		flag = get_argument(argv[i], "--stutter-chance=");
		if (flag != NULL)
		{
			char *end;
			errno = 0;

			instance->stutter_chance = (uint8_t)strtoul(flag, &end, 10);
			if (errno == ERANGE || end == flag || *end != '\0')
			{
				printf("Expected '--stutter-chance=[0-256]', got '%s'\n",
				       argv[i]);
				exit(1);
			}
			continue;
		}

		flag = get_argument(argv[i], "--rng-seed=");
		if (flag != NULL)
		{
			char *end;
			errno = 0;

			instance->rng = (uint64_t)strtoul(flag, &end, 10);
			if (errno == ERANGE || end == flag || *end != '\0')
			{
				printf("Expected '--rng-seed=[0+]', got '%s'\n", argv[i]);
				exit(1);
			}
			continue;
		}

		if ((strcmp(argv[i], "--stdin") == 0))
		{
			read_stdin = true;
			continue;
		}

		if ((strcmp(argv[i], "--help") == 0))
		{
			print_help(argv[0]);
			exit(0);
		}

		if (input_index == DEFAULT_INPUT_INDEX)
			input_index = i;
		else
		{
			printf("Unknown argument: %s\n", argv[i]);
			exit(1);
		}
	}

	if (input_index == DEFAULT_INPUT_INDEX)
	{
		print_help(argv[0]);
		exit(1);
	}
}

int main(int _, char *argv[])
{
	int ret = 0;
	uwu_instance instance = {0};

	if (uwu_init(&instance))
	{
		uwu_perrwu(&instance, "Failed to initalize libuwu");
		return 2;
	}

	if (uwu_replacement_load_defaults(&instance))
	{
		uwu_perrwu(&instance, "Failed to load replacements");
		instance.errwu = ""; // don't exit, still claer errwu
		ret = 4;             // return '4' later when the program ends
	}

	instance.stutter_chance = DEFAULT_STUTTER_CHANCE;
	instance.rng = (uint64_t)time(NULL); // seed RNG used for stuttering

	parse_arguments(argv, &instance);

	char *out;
	if (read_stdin)
	{
		char *input = write_entire_stdin_to_string();
		out = uwu_uwuify(&instance, input);
		free(input);
	}
	else
	{
		out = uwu_uwuify(&instance, argv[input_index]);
	}

	if (out == NULL)
	{
		uwu_perrwu(&instance, "uwuify");
		return 3;
	}

	printf("%s\n", out);

	free(out);
	return ret;
}
