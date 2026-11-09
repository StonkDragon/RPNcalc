CC = clang
CFLAGS ?= -Wall -Wextra -std=c17
BUILD_DIR := build
TARGET := $(BUILD_DIR)/rpncalc
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
LDFLAGS ?= -lm

all: $(TARGET)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
