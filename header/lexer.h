#ifndef LEXER_H
#define LEXER_H

#define TOKEN_MAX_LENGTH 100

struct Token {
    char token[TOKEN_MAX_LENGTH];
    char type[TOKEN_MAX_LENGTH];
};

extern struct Token *tokens;
extern int TOKEN_COUNT;

void add_token(const char *token, const char *type);

#endif
