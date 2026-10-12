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

#include "h/case.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef enum uwu_Case
{
	UWU_CASE_NO_CAPS,    // the first character is not capitalized
	UWU_CASE_FIRST_CAPS, // the first but not second character is
	                     // capitalized
	UWU_CASE_ALL_CAPS,   // all characters are capitalized
	UWU_CASE_DO_NOTHING, // string is null or empty
} uwu_Case;

void uwu_make_lowercase(char **string)
{
	char *p;

	if (string == NULL || *string == NULL)
		return;

	for (p = *string; *p != '\0'; ++p)
		*p = (char)tolower((unsigned char)*p);
}

static inline void make_upperchar(char *ch)
{
	if (ch != NULL)
		*ch = (char)toupper((unsigned char)*ch);
}

static inline void make_uppercase(char **string)
{
	char *p;

	if (string == NULL || *string == NULL)
		return;

	for (p = *string; *p != '\0'; ++p)
		make_upperchar(p);
}

// AI used here for optimization and code review and whatnot
// (because I was originally convinced this was mutating another string somehow)
bool uwu_is_same_string_nocase(const char *a, const char *b)
{
	while (*a && *b)
	{
		char ca = *a++;
		char cb = *b++;

		if (ca >= 'a' && ca <= 'z')
			ca -= 'a' - 'A';

		if (cb >= 'a' && cb <= 'z')
			cb -= 'a' - 'A';

		if (ca != cb)
			return false;
	}

	return *a == *b;
}

static inline bool is_uppercase(char ch)
{
	return ch >= 'A' && ch <= 'Z';
}

static inline uwu_Case determine_case(const char *input)
{
	bool first_caps = false;
	bool all_caps = true;

	if (input == NULL || input[0] == '\0')
		return UWU_CASE_DO_NOTHING;

	first_caps = is_uppercase(input[0]);

	for (size_t i = 0; input[i] != '\0'; ++i)
	{
		if (!is_uppercase(input[i]))
		{
			all_caps = false;
			break;
		}
	}

	if (all_caps)
		return UWU_CASE_ALL_CAPS;

	if (first_caps)
		return UWU_CASE_FIRST_CAPS;

	return UWU_CASE_NO_CAPS;
}

static bool has_non_ascii(const char *input)
{
	for (; *input != '\0'; ++input)
	{
		if ((unsigned char)*input > 127)
			return true;
	}

	return false;
}

// NOTE: mutates 'string'
void uwu_transfer_case(const char *reference, char **string)
{
	if (reference == NULL || string == NULL || *string == NULL)
		return;

	if (has_non_ascii(*string))
		return;

	uwu_Case determined_case = determine_case(reference);
	switch (determined_case)
	{
	case UWU_CASE_NO_CAPS:
		uwu_make_lowercase(string);
		break;
	case UWU_CASE_FIRST_CAPS:
		uwu_make_lowercase(string);
		make_upperchar(&((*string)[0]));
		break;
	case UWU_CASE_ALL_CAPS:
		make_uppercase(string);
		break;
	case UWU_CASE_DO_NOTHING:
		break;
	}
}
