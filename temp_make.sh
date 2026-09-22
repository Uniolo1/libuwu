#!/bin/sh
# SPDX-FileCopyrightText: NONE
# SPDX-License-Identifier: Unlicense

# library
gcc -Wall -Wextra -pedantic -O2 -c lib/*.c ; mv *.o out ; cd out ; ar rcs libuwuify.a *.o ; cd ..

# demo
gcc cmd/main.c -I. -Lout -luwuify -o out/uwuify --std=c99
