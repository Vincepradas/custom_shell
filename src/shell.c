#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

// custom cmds
int cmd_exit(int argc, char *argv[])
{
    printf("Exiting shell...\n");
    exit(0);
}

typedef struct
{
    const char *name;
    const char *desc;
    int (*func)(int argc, char *argv[]);
} Builtin;

Builtin builtin_cmds[] = {
    {"exit", "Exit the shell", cmd_exit},
    {"greet", "Say hello to arguments", cmd_greet},
    {"help", "Show list of built-in commands", cmd_listCmd}};

int num_builtins = sizeof(builtin_cmds) / sizeof(Builtin);

int cmd_greet(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "\033[31mError: Format -> greet <name>\033[0m\n");
        return 1;
    }

    printf("Hello!");
    for (int i = 1; i < argc; i++)
        printf(" %s", argv[i]);
    printf("\n");
    return 0;
}

int cmd_listCmd(int argc, char *argv[])
{
    printf("\33[32m\nLIST OF CUSTOM COMMANDS\33[0m\n");
    for (int i = 0; i < num_builtins; i++)
    {
        printf("- %s: %s\n", builtin_cmds[i].name, builtin_cmds[i].desc);
    }
    return 0;
}

// Dispatcher
int execute_builtin(Command *cmd)
{
    for (int i = 0; i < num_builtins; i++)
    {
        if (strcmp(cmd->command, builtin_cmds[i].name) == 0)
            return builtin_cmds[i].func(cmd->argc, cmd->args);
    }
    return -1; // not a custom cmd
}