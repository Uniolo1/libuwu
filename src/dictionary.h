// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>

#include "uwuify.h"

Dictionary *dict_create(size_t inital_capacity);
int dict_set(Dictionary *dict, const char *key, const char *value);
char *dict_get(Dictionary *dict, const char *key);
void dict_free(Dictionary *dict);

#endif
