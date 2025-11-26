#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "utils/token.h"
#include "utils/gmail.h"

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