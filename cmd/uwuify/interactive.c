// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software,
 * either in source code form or as a compiled binary, for any purpose, commercial or
 * non-commercial, and by any means.

 * In jurisdictions that recognize copyright laws, the author or authors of this software
 * dedicate any and all copyright interest in the software to the public domain. We make this
 * dedication for the benefit of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of relinquishment in perpetuity of
 * all present and future rights to this software under copyright law.

 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
 * OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
		size = 128;
		bufptr = malloc(size);
		if (bufptr == NULL)
		{
			return -1;
		}
	}
	p = bufptr;

	while (c != EOF)
	{
		if (p >= bufptr && (size_t)(p - bufptr) > size - 1)
		{
			size = size * 2;
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
