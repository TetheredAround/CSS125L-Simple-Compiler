%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/lexer.h"
#include "parser.tab.h"

struct Token *tokens = NULL;
int TOKEN_COUNT = 0;

extern YYSTYPE yylval;

void add_token(const char *token, const char *type) {
    if (TOKEN_COUNT % 10 == 0) {
        tokens = realloc(tokens, (TOKEN_COUNT + 10) * sizeof(struct Token));
        if (tokens == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(1);
        }
    }

    strncpy(tokens[TOKEN_COUNT].token, token, sizeof(tokens[TOKEN_COUNT].token) - 1);
    strncpy(tokens[TOKEN_COUNT].type, type, sizeof(tokens[TOKEN_COUNT].type) - 1);
    tokens[TOKEN_COUNT].token[sizeof(tokens[TOKEN_COUNT].token) - 1] = '\0';
    tokens[TOKEN_COUNT].type[sizeof(tokens[TOKEN_COUNT].type) - 1] = '\0';

    TOKEN_COUNT++;
}
%}

%%
"cin>>"        { add_token(yytext, "INPUT"); return INPUT; }
"cout<<"       { add_token(yytext, "OUTPUT"); return OUTPUT; }
[0-9]+       { yylval.ival = atoi(yytext); add_token(yytext, "INTEGER"); return INTEGER; }
"="          { add_token(yytext, "ASSIGN"); return ASSIGN; }
"+"          { add_token(yytext, "PLUS"); return PLUS; }
\"[^\"]*\"  { yylval.sval = strdup(yytext); add_token(yytext, "STRING"); return STRING; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); add_token(yytext, "IDENTIFIER"); return IDENTIFIER; }
";"          { add_token(yytext, "SEMICOLON"); return SEMICOLON; }
[ \t\n]+     {}
.            { printf("Unrecognized character: %s\n", yytext); }
%%

int yywrap() {
    return 1;
}
