// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include "../libuwu.h"

#ifndef UWUIFY_H
#define UWUIFY_H
#include <stdlib.h>

typedef struct uwu_internal_Node
{
	char *key;
	char *value;
	struct uwu_internal_Node *next;
} uwu_internal_Node;

typedef struct uwu_internal_Dictionary
{
	uwu_internal_Node **buckets; // Array of linked list pointers
	size_t capacity;             // Total number of buckets
	size_t size;                 // Current number of elements
} uwu_internal_Dictionary;

struct uwu_internal_instance_internal
{
	uwu_internal_Dictionary *replacement_dictionary;
	uint64_t (*rng_function)(uint64_t *);
	bool initalized;
};
#endif
