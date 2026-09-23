// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>

#include "uwuify.h"

Dictionary *uwu_dict_create(size_t inital_capacity);
int uwu_dict_set(Dictionary *dict, const char *key, const char *value);
char *uwu_dict_get(Dictionary *dict, const char *key);
void uwu_dict_free(Dictionary *dict);

// declared here cuz first used here:
char *uwu_strdup(const char *input);

#endif
