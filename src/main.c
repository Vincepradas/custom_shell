#include <stdio.h>
#include <string.h>
#include "executor.h"
#include "parser.h"
#include "shell.h"

int main() {
    char input[MAX_INPUT];
    Command cmd;

    while (1) {
        printf("$ ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = 0;

        parse_input(input, &cmd);

        // check built-in command
        if (execute_builtin(&cmd) == -1) {
            // Not built-in → execute as external cmd
            execute_command(&cmd);
        }
    }

    printf("Bye!\n");
    return 0;
}
