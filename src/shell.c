#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "utils/token.h"
#include "utils/gmail.h"
#include <unistd.h>

// custom cmds
typedef struct
{
    const char *name;
    const char *desc;
    int (*func)(int argc, char *argv[]);
} Builtin;

Builtin builtin_cmds[] = {
    {"exit", "Exit the shell", cmd_exit},
    {"greet", "Say hello to arguments", cmd_greet},
    {"help", "Show list of built-in commands", cmd_listCmd},
    {"cd", "Change directory", cmd_cd},
    {"ls ", "Lists files and directories in the current location", NULL},
    {"mail", "Fetches your emails today", cmd_mail}};

int num_builtins = sizeof(builtin_cmds) / sizeof(Builtin);

int cmd_exit(int argc, char *argv[])
{
    printf("Exiting shell...\n");
    exit(0);
}

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

int cmd_mail(int argc, char *argv[])
{
    const char *client_id = getenv("CLIENT_ID");
    const char *client_secret = getenv("CLIENT_SECRET");
    char *access_token = NULL;

    // Check token
    FILE *f = fopen("refresh_token.json", "r");
    if (!f)
    {
        printf("\n\033[33mNo refresh token found. First-time authorization required.\033[0m\n");

        printf("Open this URL in your browser:\n");
        printf(
            "https://accounts.google.com/o/oauth2/v2/auth?"
            "response_type=code&client_id=%s&redirect_uri=urn:ietf:wg:oauth:2.0:oob"
            "&scope=https://www.googleapis.com/auth/gmail.readonly&access_type=offline\n\n",
            client_id);

        char code[256];
        printf("Paste the authorization code here: ");
        scanf("%255s", code);

        // Exchange code for refresh token
        if (!exchange_auth_code(code, client_id, client_secret))
        {
            printf("Failed to exchange authorization code.\n");
            return 1;
        }

        printf("\033[32mRefresh token saved successfully!\033[0m\n");
    }
    else
    {
        fclose(f);
    }

    // Get refresh token
    access_token = get_access_token(client_id, client_secret);
    if (!access_token)
    {
        printf("Cannot get Gmail access token.\n");
        return 1;
    }

    printf("\n\033[34mFetching today's emails...\033[0m\n");
    fetch_todays_emails(access_token);

    free(access_token);
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