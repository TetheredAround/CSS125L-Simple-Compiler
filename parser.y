%{
#include <stdio.h>
#include <stdlib.h>
#include "header/ast.h"
#include "header/symbol_table.h"

ASTNode* program = NULL;

int yylex(void);
void yyerror(const char *s);
%}

%token <ival> INTEGER
%token <sval> IDENTIFIER
%token <sval> STRING
%token ASSIGN PLUS INPUT OUTPUT SEMICOLON

%code requires {
    #include "header/ast.h"
}

%union {
    int ival;
    char* sval;
    ASTNode* node;
}

%type <node> program statement declaration_statement initialization_statement input_statement output_statement addition_statement cout

%%

program:
    statement
    {
        program = createASTNode("program", "");
        
        addChild(program, $1);  
    }
    | statement program
    {
        addChild(program, $1);
        $$ = program;
    }
    ;


statement:
    declaration_statement
    {
        $$ = $1;
    }
    | initialization_statement
    {
        $$ = $1;
    }
    | input_statement
    {
        $$ = $1;
    }
    | output_statement
    {
        $$ = $1;
    }
    | addition_statement
    {
        $$ = $1;
    }
    ;

declaration_statement:
    IDENTIFIER SEMICOLON
    {
        // ADD DACLARATION
        addSymbol($1, "");

        $$ = createASTNode("declaration", $1);
        free($1);
    }
    ;

initialization_statement:
    IDENTIFIER ASSIGN INTEGER SEMICOLON
    {
        // INITIALIZIATION CHECK
        if (isInitialized($1)) {
            yyerror("Variable already declared.");
        }

        // ADD INITIALIZATION
        if (addSymbol($1, "integer")) {
            setInitialized($1);
        }

        char value[20];
        sprintf(value, "%d", $3);  // Convert integer to string
        $$ = createASTNode("initialization", $1);
        addChild($$, createASTNode("integer", value));
        free($1);
    }
    | IDENTIFIER ASSIGN STRING SEMICOLON
    {
        // INITIALIZIATION CHECK
        if (addSymbol($1, "string")) {
            setInitialized($1);
        }

        $$ = createASTNode("initialization", $1);
        addChild($$, createASTNode("string", $3));
        free($1);
    }
    ;

input_statement:
    INPUT IDENTIFIER SEMICOLON
    {
        // INITIALIZIATION CHECK
        if (addSymbol($2, "CIN")) {
            setInitialized($2);
        }

        $$ = createASTNode("input", "");
        addChild($$, createASTNode("identifier", $2));
        free($2);
    }

output_statement:
    OUTPUT cout SEMICOLON
    {
        $$ = createASTNode("output", "");
        addChild($$, $2);
    };
    |
    OUTPUT cout PLUS cout SEMICOLON 
    {
        // TYPE CHECKING FOR ADDITION
        if ((strcmp($2->type, "integer") != 0 && strcmp($2->type, "identifier") != 0) || 
        (strcmp($4->type, "integer") != 0 && strcmp($4->type, "identifier") != 0)) {
            yyerror("Incompatible types for addition.");
        }

        $$ = createASTNode("output", "");
        ASTNode* additionNode = createASTNode("addition", "+");
        //addChild($$, $2);
        addChild($$, additionNode);
        addChild(additionNode, $2);
        addChild(additionNode, $4);
        //addChild($$, $4);
        
    }
    ;

cout:
    IDENTIFIER
    {
        // INITIALIZIATION CHECK
        if (!isInitialized($1)) {
            yyerror("Variable is not initialized.");
        }
        
        $$ = createASTNode("identifier", $1);
        free($1);
    }
    | INTEGER
    {
        char value[20];
        sprintf(value, "%d", $1);
        $$ = createASTNode("integer", value);
    }
    | STRING
    {
        $$ = createASTNode("string", $1);
        free($1);
    }
    ;

addition_statement:
    IDENTIFIER ASSIGN IDENTIFIER PLUS IDENTIFIER SEMICOLON
    {
        // TYPE CHECKING FOR ADDITION
        if (strcmp(getType($3), "integer") != 0 || strcmp(getType($5), "integer") != 0 ) {
            yyerror("Incompatible types for addition.");
        }

        $$ = createASTNode("addition", $1);
        addChild($$, createASTNode("identifier", $3));
        addChild($$, createASTNode("identifier", $5));
        free($1);
        free($3);
        free($5);
    }
    | IDENTIFIER ASSIGN INTEGER PLUS INTEGER SEMICOLON
    {
        char value1[20];
        sprintf(value1, "%d", $3);
        char value2[20];
        sprintf(value2, "%d", $5);
        $$ = createASTNode("addition", $1);
        addChild($$, createASTNode("integer", value1));
        addChild($$, createASTNode("integer", value2));
        free($1);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
