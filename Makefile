# SPDX-FileCopyrightText: NONE
# SPDX-License-Identifier: Unlicense

CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -O2
AR      := ar
ARFLAGS := rcs

LIB_NAME := libuwu

OUT      := out
OBJ_DIR  := $(OUT)/o

LIB_SRCS := $(wildcard src/*.c)
LIB_OBJS := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))

.PHONY: library shared demo clean

# Default target
library: $(OUT)/$(LIB_NAME).a
	@echo "Built: $(abspath $<)"

# Static library
$(OUT)/$(LIB_NAME).a: $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Shared library
shared: $(OUT)/$(LIB_NAME).so.1
	@echo "Built: $(abspath $<)"

$(OUT)/$(LIB_NAME).so.1: $(LIB_SRCS)
	$(CC) $(CFLAGS) -fPIC -shared -Wl,-soname,$@ -o $@ $^

# Compile library objects
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Demo
demo: $(OUT)/$(LIB_NAME).a
	$(CC) --std=c99 cmd/main.c -I. -L$(OUT) -luwu -o $(OUT)/uwuify
	@echo "Built: $(abspath $(OUT)/uwuify)"

clean:
	rm -rf $(OUT)
