#include <libuwuify.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	uwuify_instance instance;
	if (uwu_init(&instance)) {
		puts("Failed to initalize libuwuify");
		return 2;
	}

	// return value ignored
	uwu_replacement_load_defaults(&instance);

	uwu_update_stutter_chance(&instance, 1);

	switch (argc) {
		case 0:
			puts("How???");
			break;
		case 1:
			printf("Usage: %s \"<input>\"", argv[0]);
			break;
		case 2:
			char *out = uwu_uwuify(&instance, argv[1]);
			if (out != NULL) printf("%s\n", out);
			break;
		default:
			puts("Recived too many arguments!");
			break;
	}
	return 0;
}
