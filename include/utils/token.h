#ifndef TOKEN_H
#define TOKEN_H

int exchange_auth_code(const char *code, const char *client_id, const char *client_secret);
char *get_access_token(const char *client_id, const char *client_secret);

#endif
