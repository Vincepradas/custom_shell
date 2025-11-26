# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iinclude

# Source and build directories
SRC_DIR = src
UTILS_DIR = $(SRC_DIR)/utils
CMD_DIR = $(SRC_DIR)/cmd
OBJ_DIR = build

# Source files
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/parser.c \
      $(SRC_DIR)/executor.c \
      $(SRC_DIR)/shell.c \
      $(UTILS_DIR)/gmail.c \
      $(UTILS_DIR)/token.c \
      $(UTILS_DIR)/task.c \
      $(CMD_DIR)/cmd_exit.c \
      $(CMD_DIR)/cmd_greet.c \
      $(CMD_DIR)/cmd_listCmd.c \
	  $(CMD_DIR)/cmd_cd.c \
	  $(CMD_DIR)/cmd_task.c \
	  $(CMD_DIR)/cmd_mail.c

# Object files (mirror directory structure in build)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Executable
TARGET = main

# Default target
all: $(OBJ_DIR) $(TARGET)

# Link executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lcurl -lcjson

# Compile .c -> .o, creating directories as needed
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
