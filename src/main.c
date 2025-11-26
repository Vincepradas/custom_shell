#include <stdio.h>
#include <string.h>
#include "executor.h"
#include "parser.h"
#include "shell.h"
#include "utils/colors.h"
#include <unistd.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

int main()
{
    char input[MAX_INPUT];
    Command cmd;
    printf(CYAN"\n");
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
    printf("" RESET "");

    printf("\nRun" YELLOW " help" RESET " to see all custom commands.\n\n");

    char cwd[PATH_MAX]; // buffer for current dir
    while (1)
    {
        // get current working dir
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd failed");
            strcpy(cwd, "?");
        }

        printf("\033[36m$vinXwarren:" RESET "\033[33m%s\033[0m: ", cwd);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = 0;
        parse_input(input, &cmd);

        if (execute_builtin(&cmd) == -1)
        {
            /* If the first token of the input is "ls", append --color=auto (if not present),
               reparse to update cmd, then execute the command once. */
            char in_copy[MAX_INPUT];
            strncpy(in_copy, input, sizeof(in_copy));
            in_copy[sizeof(in_copy) - 1] = '\0';
            char *tok = strtok(in_copy, " \t");
            if (tok != NULL && strcmp(tok, "ls") == 0)
            {
                if (strstr(input, "--color=auto") == NULL)
                {
                    strncat(input, " --color=auto", sizeof(input) - strlen(input) - 1);
                    parse_input(input, &cmd);
                }
            }
            execute_command(&cmd);
        }
    }

    printf("Bye!\n");
    return 0;
}
