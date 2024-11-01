#include <stdio.h>
#include <stdlib.h>
#include "header/lexer.h"

extern FILE *yyin;
extern int yyparse();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    tokens = malloc(10 * sizeof(struct Token));
    if (tokens == NULL) {
        printf("Memory allocation failed!\n");
        fclose(yyin);
        return 1;
    }

    yyparse();

    printf("\nTokens:\n");
    for (int i = 0; i < TOKEN_COUNT; i++) {
        printf("Token: %s, Type: %s\n", tokens[i].token, tokens[i].type);
    }

    free(tokens);
    fclose(yyin);
    return 0;
}
