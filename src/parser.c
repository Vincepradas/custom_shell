#include <stdio.h>
#include <string.h>
#include "parser.h"

void parse_input(char *input, Command *cmd) {
    cmd->argc = 0;

    // Tokenize by spaces
    char *token = strtok(input, " ");

    while (token != NULL && cmd->argc < MAX_ARGS - 1) {
        cmd->args[cmd->argc++] = token;
        token = strtok(NULL, " ");
    }
    cmd->args[cmd->argc] = NULL;

    if (cmd->argc > 0)
        cmd->command = cmd->args[0];
    else
        cmd->command = NULL;
}
