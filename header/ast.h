#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ASTNode {
    char* type;
    char* value;
    struct ASTNode** children;  // Pointer to an array of child nodes
    size_t num_children;        // The number of children
} ASTNode;

ASTNode* createASTNode(const char* type, const char* value);
void addChild(ASTNode* parent, ASTNode* child);
void freeASTNode(ASTNode* node);
void printAST(ASTNode* node, int level);

#endif