// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#ifndef LIBUWUIFY_H
#define LIBUWUIFY_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Node {
	char *key;
	char *value;
	struct Node *next;
} Node;

typedef struct {
	Node **buckets;  // Array of linked list pointers
	size_t capacity; // Total number of buckets
	size_t size;     // Current number of elements
} Dictionary;

typedef struct {
	uint8_t stutter_chance;
	Dictionary *replacement_dictionary;
	uint64_t rng;
	bool initalized;
} uwuify_instance;

extern const char *uwu_INFO;      // string with info, 1 line.
extern const uint16_t version[3]; // Version info

#define DEFAULT_STUTTER_CHANCE 12 // 1 in 12

void uwu_init(uwuify_instance
                  *instance); // initalizes stuff like the replacment dictionary
void uwu_close(
    uwuify_instance *instance); // closes the replacement dictionary and stuff

char *uwu_uwuify(const char *input);

// replacement dictionary:
uint8_t uwu_replacement_remove(uwuify_instance *instance, const char *item);
uint8_t uwu_replacement_update(uwuify_instance *instance, const char *key,
                               const char *value);
uint8_t uwu_replacement_load_defaults(uwuify_instance *instance);
char *uwu_replacement_get_value(uwuify_instance *instance, const char *key);

#endif
