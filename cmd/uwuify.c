// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include <libuwu.h>

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// extra stuff used for seeding
#if defined(RUNNING_ON_POSIX) || defined(__unix__) || defined(__APPLE__) ||                   \
    defined(_POSIX_VERSION)

#define RUNNING_ON_POSIX
#include <unistd.h>

#endif

#define PID_DEFAULT 0
static uint64_t pid = PID_DEFAULT;

#define DEFAULT_STUTTER_CHANCE 6 // 1 in 6
#define DEFAULT_INPUT_INDEX -1

static bool read_stdin = false;
static bool print_seed = false;
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

static inline void generate_seed(uint64_t *seed_output)
{
	// seed 'instance.rng' - used for stuttering

#ifdef RUNNING_ON_POSIX
	pid = (uint64_t)getpid();
#endif
	*seed_output = (uint64_t)time(NULL) ^ (uint64_t)(uintptr_t)seed_output ^ pid;
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

	printf("Usage: %s <input> [options]\n\n", program_name);
	printf("Options:\n");
	printf("  --stutter-chance=<0-256>  Stutter chance (default: %d)\n",
	       DEFAULT_STUTTER_CHANCE);
	printf("  --rng-seed=<0+>           RNG seed (default: UNIX time)\n");
	printf("  --print-seed              Print RNG seed after seeding\n");
	printf("  --stdin                   Read from stdin instead of arguments\n");
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
				fprintf(stderr, "Expected '--rng-seed=[0+]', got '%s'\n",
				        argv[i]);
				exit(1);
			}
			continue;
		}

		if ((strcmp(argv[i], "--stdin") == 0))
		{
			if (read_stdin)
			{
				fprintf(stderr, "'--stdin' set twice!");
				exit(1);
			}
			read_stdin = true;
			continue;
		}

		if ((strcmp(argv[i], "--print-seed") == 0))
		{
			if (print_seed)
			{
				fprintf(stderr, "'--print-seed' set twice!");
				exit(1);
			}
			print_seed = true;
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
	generate_seed(&instance.rng); // seed RNG used for stuttering

	parse_arguments(argv, &instance);

	if (print_seed)
		printf("seed: %lu\n", instance.rng);

	char *out;
	if (read_stdin)
	{
		char *input = write_entire_stdin_to_string();
		out = uwu_uwuify_mutonly(&instance, input);
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
