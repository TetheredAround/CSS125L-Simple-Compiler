#include <stdio.h>
#include <stdlib.h>
#include "header/lexer.h"
#include "header/ast.h"
#include "header/symbol_table.h"
#include "header/assemblyGeneration.h"

extern FILE *yyin;
extern int yyparse();
extern ASTNode* program;

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

    if (yyparse() == 0) {
        printf("Parsing completed successfully.\n");

        if (program == NULL) {
            printf("AST is empty.\n");
        } else {
            printf("Generated AST:\n");
            printAST(program, 0);
        }

        initAssemblyFile();
        generateAssembly(program);
        closeAssemblyFile();

        printSymbolTable();
    } else {
        printf("Parsing failed.\n");
    }

    printf("\nTokens:\n");
    for (int i = 0; i < TOKEN_COUNT; i++) {
        printf("Token: %s, Type: %s\n", tokens[i].token, tokens[i].type);
    }
    printf("\n\n");

    free(tokens);
    fclose(yyin);
    return 0;
}
