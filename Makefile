# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iinclude

# Source and build directories
SRC_DIR = src
OBJ_DIR = build

# Source files
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/parser.c \
      $(SRC_DIR)/executor.c \
      $(SRC_DIR)/shell.c \
      $(SRC_DIR)/utils/gmail.c \
      $(SRC_DIR)/utils/token.c

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Executable
TARGET = main

# Default target
all: $(OBJ_DIR) $(TARGET)

# Link executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lcurl -lcjson

# Compile .c -> .o and create subdirectories if needed
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
