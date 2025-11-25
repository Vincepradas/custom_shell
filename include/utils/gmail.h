#ifndef GMAIL_H
#define GMAIL_H

static size_t write_callback_gmail(void *contents, size_t size, size_t nmemb, void *userp);
void fetch_todays_emails(const char *token);

#endif