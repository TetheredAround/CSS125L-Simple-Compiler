%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "header/lexer.h"

    struct Token *tokens = NULL;
    int TOKEN_COUNT = 0;

    void add_token(const char *token, const char *type) {
        if (TOKEN_COUNT % 10 == 0) {
            tokens = realloc(tokens, (TOKEN_COUNT + 10) * sizeof(struct Token));
        }
        
        strncpy(tokens[TOKEN_COUNT].token, token, sizeof(tokens[TOKEN_COUNT].token) - 1);
        strncpy(tokens[TOKEN_COUNT].type, type, sizeof(tokens[TOKEN_COUNT].type) - 1);
        tokens[TOKEN_COUNT].token[sizeof(tokens[TOKEN_COUNT].token) - 1] = '\0';
        tokens[TOKEN_COUNT].type[sizeof(tokens[TOKEN_COUNT].type) - 1] = '\0';
        TOKEN_COUNT++;
    }

%}

%%
"cin"      { add_token(yytext, "INPUT"); }
"cout"     { add_token(yytext, "OUTPUT"); }
">>"       { add_token(yytext, "RSHIFT"); }
"<<"       { add_token(yytext, "LSHIFT"); }
[0-9]+     { add_token(yytext, "CONSTANT"); }
"="        { add_token(yytext, "ASSIGNMENT"); }
"+"        { add_token(yytext, "PLUS"); }
"-"        { add_token(yytext, "MINUS"); }
"*"        { add_token(yytext, "MULTIPLY"); }
"/"        { add_token(yytext, "DIVIDE"); }
[a-zA-Z_][a-zA-Z0-9_]* { add_token(yytext, "IDENTIFIER"); }
";"        { add_token(yytext, "SEMICOLON"); }
[(){}.,*^%$#@!'":|\\/?><] { add_token(yytext, "SPECIAL"); }
%%

int yywrap() {
    return 1;
}
