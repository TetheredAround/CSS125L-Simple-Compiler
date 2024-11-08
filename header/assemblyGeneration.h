#ifndef ASSEMBLYGENERATION_H
#define ASSEMBLYGENERATION_H

#include "ast.h"

void initAssemblyFile();
void generateAssembly(ASTNode* root);
void closeAssemblyFile();
void generateProgram(ASTNode *node);
void generateInitialization(ASTNode *node);
void generateOutput(ASTNode *node);
void generateInput(ASTNode *node);

#endif
