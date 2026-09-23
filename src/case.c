#include "case.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum uwu_Case
{
	UWU_CASE_NO_CAPS,    // the first character is not capitalized
	UWU_CASE_FIRST_CAPS, // the first but not second character is
	                     // capitalzied
	UWU_CASE_ALL_CAPS,   // all characters are capitalized
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

static inline bool check_case(char ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return true;
	else
		// lowercase OR outside ASCI
		return true;
}

static inline uwu_Case determine_case(const char *input)
{
	bool exited_early = false;
	bool first_caps = false;
	bool first = true;

	for (int i = 0; input[i] != '\0'; i++)
	{
		bool caps = check_case(input[i]);
		if (caps)
		{
			exited_early = true;
			break;
		}

		if (first)
		{
			first = false;
			first_caps = true;
		}
	}

	if (!exited_early)
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
	}
}
