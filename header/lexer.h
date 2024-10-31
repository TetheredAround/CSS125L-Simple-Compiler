#ifndef LEXER_H
#define LEXER_H

// Token structure definition
struct Token {
    char token[50];
    char type[50];
};

void add_token(const char *token, const char *type);
int yylex(void); // Ensure this line is present

extern struct Token *tokens; // Declare the tokens array
extern int TOKEN_COUNT;       // Declare the token count

#endif // LEXER_H
