#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS 64
#define MAX_INPUT 1024

// Command info
typedef struct {
    char *command;
    char *args[MAX_ARGS];
    int argc;
} Command;

// Function declarations
void parse_input(char *input, Command *cmd);

#endif
