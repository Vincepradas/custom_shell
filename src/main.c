#include <stdio.h>
#include <string.h>
#include "executor.h"
#include "parser.h"

int main() {
    char input[MAX_INPUT];
    Command cmd;

    while (1) {
        printf("$ ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = 0; // remove newline

        parse_input(input, &cmd);
        execute_command(&cmd);
    }

    printf("Bye!\n");
    return 0;
}
