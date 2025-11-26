#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "utils/task.h"

Builtin builtin_cmds[] = {
    {"exit", "Exit the shell", cmd_exit},
    {"greet", "Say hello to arguments", cmd_greet},
    {"help", "Show list of built-in commands", cmd_listCmd},
    {"cd", "Change directory", cmd_cd},
    {"ls ", "Lists files and directories in the current location", NULL},
    {"mail", "Fetches your emails today", cmd_mail},
    {"task", "Create, Print or Update tasks", cmd_task}};

int num_builtins = sizeof(builtin_cmds) / sizeof(Builtin);

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