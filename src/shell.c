#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

// Built-in cmds
int cmd_exit(int argc, char *argv[]) {
    printf("Exiting shell...\n");
    exit(0);
}

int cmd_hello(int argc, char *argv[]) {
    printf("Hello!");
    for (int i = 1; i < argc; i++)
        printf(" %s", argv[i]);
    printf("\n");
    return 0;
}

// Dispatcher
int execute_builtin(Command *cmd) {
    if (strcmp(cmd->command, "exit") == 0)
        return cmd_exit(cmd->argc, cmd->args);
    if (strcmp(cmd->command, "hello") == 0)
        return cmd_hello(cmd->argc, cmd->args);

    return -1; // not a built-in
}
