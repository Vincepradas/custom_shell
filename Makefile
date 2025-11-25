CC = gcc
CFLAGS = -Wall -g -Iinclude

SRC_DIR = src
OBJ_DIR = build

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/parser.c $(SRC_DIR)/executor.c $(SRC_DIR)/shell.c
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

TARGET = main

all: $(OBJ_DIR) $(TARGET)

# Link executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# .o to build/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create build dir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
