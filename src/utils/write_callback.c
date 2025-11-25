#include <stdio.h>

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    char *buf = (char *)userp;
    size_t len = strlen(buf);
    if (len + realsize >= 16384)
        realsize = 16383 - len;
    memcpy(buf + len, contents, realsize);
    buf[len + realsize] = '\0';
    return size * nmemb;
}