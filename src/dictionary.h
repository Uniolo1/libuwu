// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>

#include "uwuify.h"

uwu_internal_Dictionary *uwu_dict_create(size_t inital_capacity);
int uwu_dict_set(uwu_internal_Dictionary *dict, const char *key, const char *value);
int uwu_dict_remove(uwu_internal_Dictionary *dict, const char *key);
char *uwu_dict_get(uwu_internal_Dictionary *dict, const char *key);
void uwu_dict_free(uwu_internal_Dictionary *dict);

// declared here cuz first used here:
char *uwu_strdup(const char *input);

#endif
