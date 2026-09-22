<!--
SPDX-FileCopyrightText: NONE
SPDX-License-Identifier: Unlicense
-->

# libuwuify

A C version of [owoify-py](https://pypi.org/project/owoify-py/). Currently quite limited.

## uwuify

Stored in `cmd/`, an example program that uses `libuwuify`.

## quickstart

```c
#include <libuwuify.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	// initalize instance
	uwuify_instance instance;
	if (uwu_init(&instance)) {
		puts("Failed to initalize libuwuify");
		return 1;
	}

	// load default replacements
	// return value can probably be safely ignored here (0 on success, 1 on failure)
	uwu_replacement_load_defaults(&instance);

	// chance the stutter chance to be 1 in every 18 messages
	uwu_update_stutter_chance(&instance, 18);

	// set customn replacement
	if (uwu_replacement_update(&instance, ":)", ":3")) {
		puts("Failed to set custom replacement!");
		return 2;
	}

	// uwuify some text!
	char *output = uwu_uwuify(&instance, "Hello I am a small pretty little uwu :)");
	if (output == NULL) {
		puts("Failed to uwuify text!")
		return 3;
	}

	// output the uwuified text
	printf("%s\n", output);

	// if your program exits here, you can probably skip this step, but otherwies make sure to close your instance!
	uwu_close(&instance);

	return 0;
}
```
