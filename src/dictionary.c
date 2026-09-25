// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

// HERE BE DRAGONS:
// AI (Google search summary / Gemini) GENERATED CODE USED HEAVILY HERE!

#include "dictionary.h"

#include "case.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *uwu_strdup(const char *input)
{
	char *ret = malloc(strlen(input) + 1);
	if (ret == NULL)
		return NULL;

	strcpy(ret, input);
	return ret;
}

static unsigned long djb2_hash(const char *str)
{
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
	{
		c = tolower(c);
		hash = ((hash << 5) + hash) + c; // hash * 33 + c
	}
	return hash;
}

// Initialize the dictionary
Dictionary *uwu_dict_create(size_t initial_capacity)
{
	Dictionary *dict = malloc(sizeof(Dictionary));
	if (!dict)
		return NULL;

	dict->capacity = initial_capacity > 0 ? initial_capacity : 16;
	dict->size = 0;
	dict->buckets = calloc(dict->capacity, sizeof(Node *));

	if (!dict->buckets)
	{
		free(dict);
		return NULL;
	}
	return dict;
}

static int uwu_dict_resize(Dictionary *dict, size_t new_capacity)
{
	Node **old_buckets = dict->buckets;
	size_t old_capacity = dict->capacity;

	dict->buckets = calloc(new_capacity, sizeof(Node *));
	if (!dict->buckets)
	{
		dict->buckets = old_buckets; // Revert on failure
		return 1;
	}

	dict->capacity = new_capacity;
	dict->size = 0; // Will be re-counted as we re-insert

	for (size_t i = 0; i < old_capacity; i++)
	{
		Node *curr = old_buckets[i];
		while (curr)
		{
			uwu_dict_set(dict, curr->key, curr->value);
			Node *temp = curr;
			curr = curr->next;
			free(temp->key);
			free(temp->value);
			free(temp);
		}
	}
	free(old_buckets);
	return 0;
}

// Insert or update a key-value pair
int uwu_dict_set(Dictionary *dict, const char *key, const char *value)
{
	// Resize if load factor exceeds 0.75
	if ((float)dict->size / dict->capacity > 0.75)
	{
		if (uwu_dict_resize(dict, dict->capacity * 2))
			return 1;
	}

	unsigned long slot = djb2_hash(key) % dict->capacity;
	Node *curr = dict->buckets[slot];

	// Check if key already exists, update value if so
	while (curr)
	{
		if (strcmp(curr->key, key) == 0)
		{
			free(curr->value);
			curr->value = uwu_strdup(value);
			uwu_make_lowercase(&curr->value);
			return 0;
		}
		curr = curr->next;
	}

	// Otherwise, create a new node
	Node *new_node = malloc(sizeof(Node));
	if (!new_node)
		return 1;

	new_node->key = uwu_strdup(key);
	new_node->value = uwu_strdup(value);
	uwu_make_lowercase(&new_node->key);
	uwu_make_lowercase(&new_node->value);

	new_node->next = dict->buckets[slot];
	dict->buckets[slot] = new_node;
	dict->size++;
	return 0;
}

// Retrieve a value by key (returns NULL if not found)
char *uwu_dict_get(Dictionary *dict, const char *key)
{
	unsigned long slot = djb2_hash(key) % dict->capacity;
	Node *curr = dict->buckets[slot];

	while (curr)
	{
		if (uwu_strcasecmp(curr->key, key) == 0)
		{
			uwu_make_lowercase(&curr->value);
			return curr->value;
		}
		curr = curr->next;
	}
	return NULL;
}

// Free all memory associated with the dictionary
void uwu_dict_free(Dictionary *dict)
{
	if (!dict)
		return;
	for (size_t i = 0; i < dict->capacity; i++)
	{
		Node *curr = dict->buckets[i];
		while (curr)
		{
			Node *temp = curr;
			curr = curr->next;
			free(temp->key);
			free(temp->value);
			free(temp);
		}
	}
	free(dict->buckets);
	free(dict);
}
