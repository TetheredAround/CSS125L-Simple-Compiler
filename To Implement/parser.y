%{
#include <cstdio>
#include <cstdlib>
#include "header/ast.h"  // Your Abstract Syntax Tree (AST) header file

// Function prototype for the lexer
int yylex(void);
void yyerror(const char *s);
%}

// Token declarations
%token INTEGER IDENTIFIER ASSIGN PLUS INPUT OUTPUT SEMICOLON

// Define the union to handle multiple data types
%union {
    int ival;
    char* sval;
    ASTNode* node;
}

// Define the type for the parser rules
%type <node> program statement_list statement declaration_statement initialization_statement input_statement output_statement addition_statement

%%

program:
    statement_list
    {
        $$ = $1;
    }
    ;

statement_list:
    statement
    {
        $$ = new ASTNode("statement_list");
        $$->addChild($1);
    }
    | statement_list statement
    {
        $1->addChild($2);
        $$ = $1;
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
        $$ = new ASTNode("declaration", $1);
    }
    ;

initialization_statement:
    IDENTIFIER ASSIGN INTEGER SEMICOLON
    {
        $$ = new ASTNode("initialization", $1);
        $$->addChild(new ASTNode("integer", std::to_string($3)));
    }
    ;

input_statement:
    INPUT IDENTIFIER SEMICOLON
    {
        $$ = new ASTNode("input", $2);
    }
    ;

output_statement:
    OUTPUT IDENTIFIER SEMICOLON
    {
        $$ = new ASTNode("output", $2);
    }
    ;

addition_statement:
    IDENTIFIER ASSIGN IDENTIFIER PLUS IDENTIFIER SEMICOLON
    {
        $$ = new ASTNode("addition", $1);
        $$->addChild(new ASTNode("identifier", $3));
        $$->addChild(new ASTNode("identifier", $5));
    }
    ;

%%

// Error handling function
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(){
    return yyparse();
}
