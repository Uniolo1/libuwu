# SPDX-FileCopyrightText: NONE
# SPDX-License-Identifier: Unlicense

CC     = cc
CFLAGS = -std=c99 -Wall -Wextra -O2 -g

SRC    = $(wildcard src/*.c)
OBJ    = $(patsubst src/%.c,out/obj/%.o,$(SRC))
TARGET = out/uwuify

.PHONY: all clean strip

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@
	@echo "Ready in: $(abspath $@)"

out/obj/%.o: src/%.c
	@mkdir -p out/obj
	$(CC) $(CFLAGS) -c $< -o $@

strip: $(TARGET)
	strip $(TARGET)
	@echo "Debug symbols stripped from: $(TARGET)"

clean:
	rm -rf out
