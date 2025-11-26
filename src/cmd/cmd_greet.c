#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

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