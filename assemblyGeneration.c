#include <stdio.h>
#include <string.h>
#include "header/ast.h"
#include "header/assemblyGeneration.h"

FILE *asm_file;

void initAssemblyFile() {
    asm_file = fopen("output.asm", "w");
    if (!asm_file) {
        fprintf(stderr, "Error opening output file for assembly.\n");
        return;
    }
}

void closeAssemblyFile() {
    if (asm_file) {
        fclose(asm_file);
    }
}

void generateAssembly(ASTNode* node) {
    if (node == NULL) return;

    printf("Visiting node type: %s\n", node->type);  // Debugging line
    
    if (strcmp(node->type, "initialization") == 0) {
        printf("Generating MOV for %s with value %s\n", node->value, node->children[0]->value);
        fprintf(asm_file, "MOV %s, %s\n", node->value, node->children[0]->value);
    }
    else if (strcmp(node->type, "input") == 0) {
        printf("Generating input for %s\n", node->value);
        generateInput(node);  // Handle input
    }
    else if (strcmp(node->type, "output") == 0) {
        if (node->num_children > 0) {
            if (strcmp(node->children[0]->type, "string") == 0) {
                printf("Generating string output: %s\n", node->children[0]->value);
                fprintf(asm_file, "OUT %s\n", node->children[0]->value);  // Handle string output
            }
            else if (strcmp(node->children[0]->type, "identifier") == 0) {
                printf("Generating identifier output: %s\n", node->children[0]->value);
                fprintf(asm_file, "OUT %s\n", node->children[0]->value);  // Handle identifier output
            }
        }
    }

    for (size_t i = 0; i < node->num_children; i++) {
        generateAssembly(node->children[i]);
    }
}

void generateProgram(ASTNode *node) {
    if (!asm_file) return;  // Ensure file is valid
    fprintf(asm_file, "; Program Start\n");

    for (size_t i = 0; i < node->num_children; ++i) {
        generateAssembly(node->children[i]);
    }

    fprintf(asm_file, "; Program End\n");
}

void generateInitialization(ASTNode *node) {
    if (node->num_children > 0) {
        fprintf(asm_file, "MOV %s, %s\n", node->value, node->children[0]->value);
    }
}

void generateOutput(ASTNode *node) {
    if (node->num_children > 0) {
        fprintf(asm_file, "OUT %s\n", node->children[0]->value);
    }
}

void generateInput(ASTNode *node) {
    if (node->num_children > 0) {
        fprintf(asm_file, "IN %s\n", node->children[0]->value);
    }
}
