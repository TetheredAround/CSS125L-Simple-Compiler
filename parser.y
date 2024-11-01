%{
#include <stdio.h>
#include <stdlib.h>
#include "header/ast.h" 

int yylex(void);
void yyerror(const char *s);
%}

%token <ival> INTEGER
%token <sval> IDENTIFIER
%token ASSIGN PLUS INPUT OUTPUT SEMICOLON

%union {
    int ival;
    char* sval;
    ASTNode* node;
}

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
        free($1);
    }
    ;

initialization_statement:
    IDENTIFIER ASSIGN INTEGER SEMICOLON
    {
        $$ = new ASTNode("initialization", $1);
        $$->addChild(new ASTNode("integer", std::to_string($3)));
        free($1);
    }
    ;

input_statement:
    INPUT IDENTIFIER SEMICOLON
    {
        $$ = new ASTNode("input", $2);
        free($2);
    }
    ;

output_statement:
    OUTPUT IDENTIFIER SEMICOLON
    {
        $$ = new ASTNode("output", $2);
        free($2);
    }
    ;

addition_statement:
    IDENTIFIER ASSIGN IDENTIFIER PLUS IDENTIFIER SEMICOLON
    {
        $$ = new ASTNode("addition", $1);
        $$->addChild(new ASTNode("identifier", $3));
        $$->addChild(new ASTNode("identifier", $5));
        free($1);
        free($3);
        free($5);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
