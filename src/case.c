// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "case.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef enum uwu_Case
{
	UWU_CASE_NO_CAPS,    // the first character is not capitalized
	UWU_CASE_FIRST_CAPS, // the first but not second character is
	                     // capitalzied
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
void uwu_transfer_case(const char *refrence, char **string)
{
	if (refrence == NULL || string == NULL || *string == NULL)
		return;

	if (has_non_ascii(*string))
		return;

	uwu_Case determined_case = determine_case(refrence);
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
