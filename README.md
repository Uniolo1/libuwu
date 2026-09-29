<!--
SPDX-FileCopyrightText: NONE
SPDX-License-Identifier: Unlicense
-->

# libuwu

A library (and program) written in ISO C99 to uwuify text. Originally a Python program but then rewritten to learn how to make a C library. I have forgotten the motiviation for the original Python version.

## Building

Use `xmake` to compile to a static library, and `xmake b uwuify` to compile uwuify!

To compile and run the test suite, run `xmake test`

### Building manually

> [!NOTE]  
> The following instructions assume that you are on a UNIX-like system

If you don't have xmake, you _can_ compile it manually.

```sh
# assumes 'cc' is either Clang or GCC
mkdir -p out
cd out
cc -c ../src/*.c -O2 -std=c99
ar rcs libuwu.a *.o
cd ..
```

And then for `uwuify` (back in the current directory)

```sh
cc -I. -Lout cmd/uwuify.c -o out/uwuify -std=c99 -luwu
```

## uwuify

A program using the library, stored in `cmd/`.

## Quickstart

```c
#include <libuwu.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	// print quick information about the library
	printf("%s\n", uwu_INFO);

	// initalize instance
	uwu_instance instance = {0}; // IMPORTANT: may segfault without '= {0}'
	if (uwu_init(&instance))
	{
		// uwu_perrwu works on instances that failed to initalize
		uwu_perrwu(&instance, "Failed to initalize libuwu");
		return 1;
	}

	// load default replacements
	if (uwu_replacement_load_defaults(&instance))
	{
		// there is also uwu_fperrwu which outputs to 'stream' instead of stderr,
		// the call below is the same as uwu_perrwu but I am using it to demonstrate
		uwu_fperrwu(stderr, &instance, "Failed to set custom replacement");
		instance.errwu = NULL; // clear errwu incase another error occurs
		                       // usally a good idea (but not required) to exit here
	}

	instance.stutter_chance = 8; // chance the stutter chance to be 1 in every 8 messages
	instance.rng =
	    (uint64_t)time(NULL) ^
	    (uint64_t)(uintptr_t)&instance
	        .rng; // seed RNG used for stuttering, seed it how you would seed srand.

	// Notably, you can also change the function used to generate a random number like
	// so:
	//     uint64_t rng_next(uint64_t *state) {(*state)++; return *state;} // example
	//     function uwu_rng_change(&instance, rng_next);

	// set customn replacement
	if (uwu_replacement_update(&instance, ":(", ":)"))
	{
		uwu_perrwu(&instance, "Failed to set custom replacement");
		return 2;
	}

	// uwuify some text!
	char *output = uwu_uwuify(&instance, "Hello I am a small pretty little uwu :(");
	if (output == NULL)
	{
		uwu_perrwu(&instance, "Failed to uwuify text");
		return 3;
	}

	// output the uwuified text
	printf("%s\n", output);
	free(output); // output is malloc'd, remeber to free it!

	// if your program exits here, you can probably skip this step, but otherwies make sure
	// to close your instance!
	uwu_close(&instance);

	return 0;
}
```

Do note that this library assumes that all input is either UTF8 or ASCI encoded.
