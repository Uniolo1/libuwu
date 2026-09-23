<!--
SPDX-FileCopyrightText: NONE
SPDX-License-Identifier: Unlicense
-->

# libuwu

A version of [owoify-py](https://pypi.org/project/owoify-py/) written in ANSI C99. Originally a Python program but then rewritten to learn how to make a C library. I have forgotten the motiviation for the original Python version.

## building

Use `make` to compile to a static library, `make shared` to compile to a dynamic library, and `make demo` to compile the demo! This library should also work on Windows but you will have to compile it manually.

## uwuify

A demo program, stored in `cmd/`.

## quickstart

```c
#include <libuwuify.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	// print quick information about the library
	printf("%s\n", uwu_INFO);

	// initalize instance
	uwu_instance instance;
	if (uwu_init(&instance)) {
		uwu_perrwu(&instance, "Failed to initalize libuwu");
		return 1;
	}

	// load default replacements
	// return value can probably be safely ignored here (0 on success, 1 on failure)
	uwu_replacement_load_defaults(&instance);

	// chance the stutter chance to be 1 in every 18 messages
	uwu_update_stutter_chance(&instance, 18);

	// set customn replacement
	if (uwu_replacement_update(&instance, ":)", ":3")) {
		uwu_perrwu(&instance, "Failed to set custom replacement");
		return 2;
	}

	// uwuify some text!
	char *output = uwu_uwuify(&instance, "Hello I am a small pretty little uwu :)");
	if (output == NULL) {
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
