// SPDX-License-Identifier: Unlicense

#include <stdio.h>
#include <stdlib.h>

#include "parse.h"

// Found on stackoverflow. This function was already in the public domain.
// Thanks Will Hartung!
size_t sof_getline(char **lineptr, size_t *n, FILE *stream) {
	char *bufptr = NULL;
	char *p = bufptr;
	size_t size;
	int c;

	if (lineptr == NULL) {
		return -1;
	}
	if (stream == NULL) {
		return -1;
	}
	if (n == NULL) {
		return -1;
	}
	bufptr = *lineptr;
	size = *n;

	c = fgetc(stream);
	if (c == EOF) {
		return -1;
	}
	if (bufptr == NULL) {
		bufptr = malloc(128);
		if (bufptr == NULL) {
			return -1;
		}
		size = 128;
	}
	p = bufptr;
	while (c != EOF) {
		if ((p - bufptr) > (size - 1)) {
			size = size + 128;
			bufptr = realloc(bufptr, size);
			if (bufptr == NULL) {
				return -1;
			}
		}
		*p++ = c;
		if (c == '\n') {
			break;
		}
		c = fgetc(stream);
	}

	*p++ = '\0';
	*lineptr = bufptr;
	*n = size;

	return p - bufptr - 1;
}

void uwuify(void) {
	char *line = NULL;
	size_t size = 0;

	while (1) {
		printf(">>> ");

		size_t length = sof_getline(&line, &size, stdin);

		if (length == -1) {
			break; // EOF
		}

		printf("%s", uwuify_string(line));
	}

	free(line);
}

char *uwuify_text(char *input) { return uwuify_string(input); }

int main(int argc, char *argv[]) {
	char *text = NULL;
	switch (argc) {
		case 0:
		case 1:
			uwuify();
			return 3; // uwuify should never exit
		case 2:
			text = uwuify_text(argv[1]);
			if (text == NULL) return 2;

			printf("%s\n", text);
			break;
		case 3:
			puts("Recived too many arguments!");
			return 1;
	}
	return 0;
}
