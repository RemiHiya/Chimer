# Variables
CC = gcc
CFLAGS = -Wall -Isrc/include
LDFLAGS =
SRC_DIR = src
BUILD_DIR = build
EXECUTABLE = program

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# List of object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(EXECUTABLE)

# Rule to link the executable
$(EXECUTABLE): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Rule to compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean target
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

# Phony targets
.PHONY: all clean
