#include "header/ast.h"

// Create a new AST node
ASTNode* createASTNode(const char* type, const char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = strdup(type);
    node->value = value ? strdup(value) : NULL;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Add a child node to a parent node
void addChild(ASTNode* parent, ASTNode* child) {
    parent->num_children++;
    parent->children = (ASTNode**)realloc(parent->children, parent->num_children * sizeof(ASTNode*));
    parent->children[parent->num_children - 1] = child;
}

// Free the memory used by an AST node and its children
void freeASTNode(ASTNode* node) {
    if (node) {
        free(node->type);
        if (node->value) {
            free(node->value);
        }
        for (size_t i = 0; i < node->num_children; ++i) {
            freeASTNode(node->children[i]);
        }
        free(node->children);
        free(node);
    }
}

// Function to print the AST in a readable format
void printAST(ASTNode* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    printf("Node type: %s", node->type);
    if (node->value) {
        printf(", value: %s", node->value);
    }
    printf("\n");

    for (size_t i = 0; i < node->num_children; ++i) {
        printAST(node->children[i], level + 1);
    }
}