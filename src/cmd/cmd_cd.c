#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int cmd_cd(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "\033[31mError: cd requires a directory argument\033[0m\n");
        return 1;
    }

    if (chdir(argv[1]) != 0)
    {
        perror("cd failed");
        return 1;
    }

    return 0;
}