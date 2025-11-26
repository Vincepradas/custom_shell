#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int cmd_listCmd(int argc, char *argv[])
{
    printf("\33[32m\nLIST OF CUSTOM COMMANDS\33[0m\n");
    for (int i = 0; i < num_builtins; i++)
    {
        printf("- %s: %s\n", builtin_cmds[i].name, builtin_cmds[i].desc);
    }
    return 0;
}