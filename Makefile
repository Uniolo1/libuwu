# SPDX-FileCopyrightText: NONE
# SPDX-License-Identifier: Unlicense

CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -g -fno-omit-frame-pointer -D_ISOC99_SOURCE
AR      := ar
ARFLAGS := rcs

LIB_NAME := libuwu

OUT      := out
OBJ_DIR  := $(OUT)/o

LIB_SRCS := $(wildcard src/*.c)
LIB_OBJS := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))
ANALYSIS_DIR := $(OUT)/analysis

.PHONY: library shared uwuify clean analyze loc test

library: $(OUT)/$(LIB_NAME).a
	@echo "Built: $(abspath $<)"

$(OUT)/$(LIB_NAME).a: $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

shared: $(OUT)/$(LIB_NAME).so.2
	@echo "Built: $(abspath $<)"

$(OUT)/$(LIB_NAME).so.2: $(LIB_SRCS)
	$(CC) -O2 $(CFLAGS) -fPIC -shared -Wl,-soname,$@ -o $@ $^

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -O2 $(CFLAGS) -c $< -o $@

uwuify: $(OUT)/$(LIB_NAME).a
	$(CC) -O2 $(CFLAGS) --std=c99 cmd/uwuify.c -I. -L$(OUT) -luwu -o $(OUT)/uwuify
	@echo "Built: $(abspath $(OUT)/uwuify)"

test: $(OUT)/$(LIB_NAME).a
	$(CC) -O0 $(CFLAGS) --std=c99 tests/main.c -I. -L$(OUT) -luwu -o $(OUT)/tests
	@echo "Built: $(abspath $(OUT)/tests)"


analyze:
	@rm -rf $(ANALYSIS_DIR)
	scan-build -o $(ANALYSIS_DIR) --status-bugs $(MAKE) -B library

loc:
	# cargo install loc
	loc --exclude LICENSES/*

clean:
	rm -rf $(OUT)
