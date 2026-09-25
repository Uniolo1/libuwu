// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include <libuwu.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int main(int argc, char *argv[])
{
	uwu_instance instance;
	if (uwu_init(&instance))
	{
		uwu_perrwu(&instance, "Failed to initalize libuwu");
		return 2;
	}

	// return value is ignored here
	uwu_replacement_load_defaults(&instance);

	instance.stutter_chance = 24;
	instance.rng = (uint64_t)time(NULL); // seed RNG used for stuttering

	char *out;
	switch (argc)
	{
	case 0:
		puts("How???");
		break;
	case 1:
		printf("%s\n", uwu_INFO);
		printf("Usage: %s \"<input>\"", argv[0]);
		break;
	case 2:
		if (argv[1][0] == '-' || argv[1][1] == '\0')
		{
			char *input = write_entire_stdin_to_string();
			out = uwu_uwuify(&instance, input);
			free(input);
		}
		else
		{
			out = uwu_uwuify(&instance, argv[1]);
		}

		if (out == NULL)
		{
			uwu_perrwu(&instance, "uwuify");
			return 3;
		}
		printf("%s\n", out);
		break;
	default:
		puts("Recived too many arguments!");
		break;
	}
	return 0;
}
