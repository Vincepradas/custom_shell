#include <stdio.h>
#include <string.h>
#include "executor.h"
#include "parser.h"
#include "shell.h"

int main()
{
    char input[MAX_INPUT];
    Command cmd;
    printf("\033[36m\n");
    printf("  ╔════════════════════════════════════════════════════════════════╗\n");
    printf("  ║                        VINXWARREN, CUSTOM SHELL                ║\n");
    printf("  ╚════════════════════════════════════════════════════════════════╝\n\n");
    printf(" __ __  ____  ____   __ __  __    __   ____  ____   ____     ___  ____  \n"
           "|  |  ||    ||    \\ |  |  ||  |__|  | /    ||    \\ |    \\   /  _]|    \\ \n"
           "|  |  | |  | |  _  ||  |  ||  |  |  ||  o  ||  D  )|  D  ) /  [_ |  _  |\n"
           "|  |  | |  | |  |  ||_   _||  |  |  ||     ||    / |    / |    _]|  |  |\n"
           "|  :  | |  | |  |  ||     ||  `  '  ||  _  ||    \\ |    \\ |   [_ |  |  |\n"
           " \\   /  |  | |  |  ||  |  | \\      / |  |  ||  .  \\|  .  \\|     ||  |  |\n"
           "  \\_/  |____||__|__||__|__|  \\_/\\_/  |__|__||__|\\_||__|\\_||_____||__|__|\n");
    printf("\033[0m"); 

    printf("\nRun \033[33mhelp\033[0m to see all custom commands.\n\n");

    while (1)
    {

        printf("\033[32m$vinXwarren: \033[0m");
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;
        input[strcspn(input, "\n")] = 0;

        parse_input(input, &cmd);

        // check built-in command
        if (execute_builtin(&cmd) == -1)
        {
            // Not built-in → execute as external cmd
            execute_command(&cmd);
        }
    }

    printf("Bye!\n");
    return 0;
}
