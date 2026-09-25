// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include <libuwu.h>
#include <stdio.h>
#include <time.h>

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
		out = uwu_uwuify(&instance, argv[1]);
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
