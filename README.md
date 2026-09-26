<!--
SPDX-FileCopyrightText: NONE
SPDX-License-Identifier: Unlicense
-->

# libuwu

A version of [owoify-py](https://pypi.org/project/owoify-py/) written in ISO C99. Originally a Python program but then rewritten to learn how to make a C library. I have forgotten the motiviation for the original Python version.

## building

Use `make` to compile to a static library, `make shared` to compile to a dynamic library, and `make demo` to compile the demo! This library should also work on Windows but you will have to compile it manually (shouldn't be too hard).

## uwuify

A demo program, stored in `cmd/`.

## quickstart

```c
#include <libuwuify.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
	// print quick information about the library
	printf("%s\n", uwu_INFO);

	// initalize instance
	uwu_instance instance;
	if (uwu_init(&instance))
	{
		// uwu_perrwu works on instances that failed to initalize
		uwu_perrwu(&instance, "Failed to initalize libuwu");
		return 1;
	}

	// load default replacements
	if (uwu_replacement_load_defaults(&instance))
	{
		// print to stderr here since we don't exit
		uwu_fperrwu(stderr, &instance, "Failed to set custom replacement");
		instance.errwu = NULL; // clear errwu incase another error occurs
		// usally a good idea (but not required) to exit here
	}

	instance.stutter_chance = 8; // chance the stutter chance to be 1 in every 8 messages
	instance.rng = (uint64_t)time(NULL); // seed RNG used for stuttering, seed it how you would seed srand.

	// Notably, you can also change the function used to generate a random number like so:
	//     uint64_t rng_next(uint64_t *state) {(void)state; return 0;} // example function
	//     uwu_rng_change(&instance, rng_next);

	// set customn replacement
	if (uwu_replacement_update(&instance, ":(", ":)"))
	{
		// print to stderr here since we don't exit
		uwu_fperrwu(stderr, &instance, "Failed to set custom replacement");
		return 2;
	}

	// uwuify some text!
	char *output = uwu_uwuify(&instance, "Hello I am a small pretty little uwu :(");
	if (output == NULL)
	{
		uwu_perrwu(&instance, "Failed to uwuify text")
		return 3;
	}

	// output the uwuified text
	printf("%s\n", output);

	// if your program exits here, you can probably skip this step, but otherwies make sure to close your instance!
	uwu_close(&instance);

	return 0;
}
```
