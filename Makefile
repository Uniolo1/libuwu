# SPDX-FileCopyrightText: NONE
# SPDX-License-Identifier: Unlicense

CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -O2 -g -fno-omit-frame-pointer
AR      := ar
ARFLAGS := rcs

LIB_NAME := libuwu

OUT      := out
OBJ_DIR  := $(OUT)/o

LIB_SRCS := $(wildcard src/*.c)
LIB_OBJS := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))
ANALYSIS_DIR := $(OUT)/analysis

.PHONY: library shared demo clean analyze

library: $(OUT)/$(LIB_NAME).a
	@echo "Built: $(abspath $<)"

$(OUT)/$(LIB_NAME).a: $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

shared: $(OUT)/$(LIB_NAME).so.1
	@echo "Built: $(abspath $<)"

$(OUT)/$(LIB_NAME).so.1: $(LIB_SRCS)
	$(CC) $(CFLAGS) -fPIC -shared -Wl,-soname,$@ -o $@ $^

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

demo: $(OUT)/$(LIB_NAME).a
	$(CC) $(CFLAGS) --std=c99 cmd/main.c -I. -L$(OUT) -luwu -o $(OUT)/uwuify
	@echo "Built: $(abspath $(OUT)/uwuify)"

analyze:
	@rm -rf $(ANALYSIS_DIR)
	scan-build -o $(ANALYSIS_DIR) --status-bugs $(MAKE) -B library

clean:
	rm -rf $(OUT)
