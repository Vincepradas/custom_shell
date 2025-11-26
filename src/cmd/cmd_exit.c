#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int cmd_exit(int argc, char *argv[])
{
    printf("Exiting shell...\n");
    exit(0);
}