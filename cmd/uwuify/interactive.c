// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include <libuwu.h>
#include <stdio.h>
#include <stdlib.h>

#include "interactive.h"

static size_t sof_getline(char **lineptr, size_t *n, FILE *stream);

void handle_keyboard_exit(int sig)
{
	// here to avoid nushell from throwing an ugly error
	(void)sig;

	// exit(0) here wouldn't be portable, not that big of a deal either way though
	_Exit(0);
}

int interactive_mode(void)
{
	char *line = NULL;
	size_t size = 0;
	int ret = 0;

	while (1)
	{
		printf(">>> ");

		size_t length = sof_getline(&line, &size, stdin);

		if (length <= 0)
		{
			ret = 3;
			break;
		}

		if (line[0] == '\n' || line[0] == '\0')
			break;

		char *uwuified = uwu_uwuify_mutonly(&instance, line);
		if (uwuified == NULL)
		{
			ret = 2;
			break;
		}
		printf("%s", uwuified);
	}

	free(line);
	return ret;
}

// Found on stackoverflow (modifed it). This function was already in the public
// domain, thought it did not have any comments and I changed some of the
// formatting. Thanks Will Hartung!
static size_t sof_getline(char **lineptr, size_t *n, FILE *stream)
{
	char *bufptr = NULL;
	char *p = bufptr;
	size_t size;
	int c;

	// ensure none of the arguments are NULL
	if (lineptr == NULL)
		return -1;
	if (stream == NULL)
		return -1;
	if (n == NULL)
		return -1;

	bufptr = *lineptr;
	size = *n;

	c = fgetc(stream);
	if (c == EOF)
		return -1;

	if (bufptr == NULL)
	{
		// allocate the bufptr if it is NULL
		bufptr = malloc(128);
		if (bufptr == NULL)
		{
			return -1;
		}
		size = 128;
	}
	p = bufptr;

	while (c != EOF)
	{
		if (p >= bufptr && (size_t)(p - bufptr) > size - 1)
		{
			size = size + 128;
			bufptr = realloc(bufptr, size);
			if (bufptr == NULL)
			{
				return -1;
			}
		}
		*p++ = c;
		if (c == '\n')
		{
			break;
		}
		c = fgetc(stream);
	}

	*p++ = '\0';
	*lineptr = bufptr;
	*n = size;

	return p - bufptr - 1;
}
