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

// uwu instance
typedef struct {
	uint8_t stutter_chance;
	Dictionary *replacement_dictionary;
	uint64_t rng;
	bool initalized;
	char *errwu;
} uwu_instance;

extern const char *uwu_INFO;          // string with info, 1 line.
extern const uint16_t uwu_VERSION[3]; // Version info

#define DEFAULT_STUTTER_CHANCE 24 // 1 in 12

uint8_t uwu_init(uwu_instance *instance); // initalizes stuff like the
                                          // replacment dictionary
void uwu_close(
    uwu_instance *instance); // closes the replacement dictionary and stuff

char *uwu_uwuify(uwu_instance *instance, char *input);

// utilities:
void uwu_update_stutter_chance(uwu_instance *instance, uint8_t new_chance);
void uwu_print_errwu(char *input, uwu_instance *instance);

// replacement dictionary:
uint8_t uwu_replacement_remove(uwu_instance *instance, const char *item);
uint8_t uwu_replacement_update(uwu_instance *instance, const char *key,
                               const char *value);
uint8_t uwu_replacement_load_defaults(uwu_instance *instance);
char *uwu_replacement_get_value(uwu_instance *instance, const char *key);

#endif
