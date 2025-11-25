#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#define TOKEN_FILE "refresh_token.json"


static size_t write_callback_token(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *buf = (char *)userp;
    size_t len = strlen(buf);
    if (len + realsize >= 8192) realsize = 8191 - len;
    memcpy(buf + len, contents, realsize);
    buf[len + realsize] = '\0';
    return realsize;
}

// auth code for refresh token
int exchange_auth_code(const char *code, const char *client_id, const char *client_secret)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return 0;

    char postfields[1024];  
    snprintf(postfields, sizeof(postfields),
             "code=%s&client_id=%s&client_secret=%s&redirect_uri=urn:ietf:wg:oauth:2.0:oob&grant_type=authorization_code",
             code, client_id, client_secret);

    char buffer[8192] = {0};

    curl_easy_setopt(curl, CURLOPT_URL, "https://oauth2.googleapis.com/token");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_token);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    cJSON *json = cJSON_Parse(buffer);
    if (!json)
    {
        printf("Failed to parse token response.\n");
        return 0;
    }

    cJSON *refresh = cJSON_GetObjectItem(json, "refresh_token");
    if (!refresh)
    {
        printf("No refresh token returned. Make sure 'access_type=offline' is set.\n");
        cJSON_Delete(json);
        return 0;
    }

    FILE *f = fopen(TOKEN_FILE, "w");
    if (!f)
    {
        perror("fopen");
        cJSON_Delete(json);
        return 0;
    }
    fprintf(f, "%s", refresh->valuestring);
    fclose(f);

    cJSON_Delete(json);
    printf("Refresh token saved to %s\n", TOKEN_FILE);
    return 1;
}

// Get access token
char *get_access_token(const char *client_id, const char *client_secret)
{
    FILE *f = fopen(TOKEN_FILE, "r");
    if (!f)
    {
        printf("No refresh token found. You need to authorize first.\n");
        return NULL;
    }

    char refresh_token[1024];
    fgets(refresh_token, sizeof(refresh_token), f);
    fclose(f);
    refresh_token[strcspn(refresh_token, "\n")] = 0;

    CURL *curl = curl_easy_init();
    if (!curl)
        return NULL;

    char postfields[4096];
    snprintf(postfields, sizeof(postfields),
             "client_id=%s&client_secret=%s&refresh_token=%s&grant_type=refresh_token",
             client_id, client_secret, refresh_token);

    char buffer[8192] = {0};
    curl_easy_setopt(curl, CURLOPT_URL, "https://oauth2.googleapis.com/token");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_token);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    cJSON *json = cJSON_Parse(buffer);
    if (!json)
    {
        printf("Failed to parse access token response.\n");
        return NULL;
    }

    cJSON *token = cJSON_GetObjectItem(json, "access_token");
    if (!token)
    {
        printf("No access token received.\n");
        cJSON_Delete(json);
        return NULL;
    }

    char *copy = strdup(token->valuestring);
    cJSON_Delete(json);
    return copy;
}
