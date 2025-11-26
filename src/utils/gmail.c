#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>

size_t write_callback_gmail(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *buf = (char *)userp;
    size_t len = strlen(buf);
    if (len + realsize >= 16384) realsize = 16383 - len;
    memcpy(buf + len, contents, realsize);
    buf[len + realsize] = '\0';
    return realsize;
}


void fetch_todays_emails(const char *access_token) {
    CURL *curl = curl_easy_init();
    if (!curl) return;

    char buffer[16384] = {0};
    struct curl_slist *headers = NULL;
    char auth_header[512];
    sprintf(auth_header, "Authorization: Bearer %s", access_token);
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL,
        "https://gmail.googleapis.com/gmail/v1/users/me/messages?q=newer_than:1d");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_gmail);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    cJSON *json = cJSON_Parse(buffer);
    if (!json) { printf("Failed to parse Gmail response.\n"); return; }

    cJSON *messages = cJSON_GetObjectItem(json, "messages");
    if (!messages) { printf("No emails found today.\n"); cJSON_Delete(json); return; }

    for (int i = 0; i < cJSON_GetArraySize(messages); i++) {
        cJSON *msg = cJSON_GetArrayItem(messages, i);
        const char *id = cJSON_GetObjectItem(msg, "id")->valuestring;

        // Fetch each message details
        char url[512];
        snprintf(url, sizeof(url),
            "https://gmail.googleapis.com/gmail/v1/users/me/messages/%s?format=metadata&metadataHeaders=From&metadataHeaders=Subject",
            id);

        CURL *mcurl = curl_easy_init();
        char msgbuf[8192] = {0};
        struct curl_slist *mheaders = NULL;
        mheaders = curl_slist_append(mheaders, auth_header);
        curl_easy_setopt(mcurl, CURLOPT_URL, url);
        curl_easy_setopt(mcurl, CURLOPT_HTTPHEADER, mheaders);
        curl_easy_setopt(mcurl, CURLOPT_WRITEFUNCTION, write_callback_gmail);
        curl_easy_setopt(mcurl, CURLOPT_WRITEDATA, msgbuf);
        curl_easy_perform(mcurl);
        curl_easy_cleanup(mcurl);
        curl_slist_free_all(mheaders);

        cJSON *mjson = cJSON_Parse(msgbuf);
        if (!mjson) continue;

        cJSON *payload = cJSON_GetObjectItem(mjson, "payload");
        cJSON *headers = cJSON_GetObjectItem(payload, "headers");
        const char *from = "", *subject = "";
        for (int j = 0; j < cJSON_GetArraySize(headers); j++) {
            cJSON *h = cJSON_GetArrayItem(headers, j);
            const char *name = cJSON_GetObjectItem(h, "name")->valuestring;
            const char *value = cJSON_GetObjectItem(h, "value")->valuestring;
            if (strcmp(name, "From") == 0) from = value;
            if (strcmp(name, "Subject") == 0) subject = value;
        }

        printf("From: %s | Subject: %s\n", from, subject);
        cJSON_Delete(mjson);
    }

    cJSON_Delete(json);
}
