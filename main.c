#include <stdio.h>
#include <stdlib.h>
#include "header/lexer.h"
#include "header/ast.h"
#include "header/symbol_table.h"

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>

extern FILE *yyin;
extern int yyparse();
extern ASTNode* program;

LLVMModuleRef module;
LLVMBuilderRef builder;
LLVMContextRef context;

void initializeLLVM(){
    context = LLVMContextCreate();
    module = LLVMModuleCreateWithNameInContext("my_mod", context);
    builder = LLVMCreateBuilderInContext(context);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    LLVMTypeRef funcType = LLVMFunctionType(LLVMVoidTypeInContext(context), NULL, 0, 0);
    LLVMValueRef function = LLVMAddFunction(module, "main", funcType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(context, function, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
}

void finalizeIR(){
    char *error = NULL;

    LLVMBuildRetVoid(builder); 

    if (LLVMPrintModuleToFile(module, "output.ll", NULL) != 0) {
        fprintf(stderr, "Error: Could not write IR to output.ll\n");
    }

    LLVMExecutionEngineRef engine;
    LLVMCreateExecutionEngineForModule(&engine, module, &error);
    if (error){
        fprintf(stderr, "Error creating execution engine: %s\n", error);
        exit(1);
    }
}

void generateIR(ASTNode* node, int level, LLVMBuilderRef builder, LLVMModuleRef module, LLVMContextRef context) {
    if (!node) return;

    // Process initialization nodes
    if (strcmp(node->type, "initialization") == 0) {
        printf("Initializing variable %s...\n", node->value);

        LLVMTypeRef intType = LLVMInt32TypeInContext(context);
        LLVMValueRef varAlloc = LLVMBuildAlloca(builder, intType, node->value);

        for (size_t i = 0; i < node->num_children; ++i) {
            ASTNode* child = node->children[i];
            if (strcmp(child->type, "integer") == 0 && child->value) {
                int initValue = atoi(child->value);  // Convert value to integer
                LLVMValueRef constValue = LLVMConstInt(intType, initValue, 0);  // Create constant integer
                LLVMBuildStore(builder, constValue, varAlloc);  // Store value in the allocated variable
            }
        }
    }

    for (size_t i = 0; i < node->num_children; ++i) {
        generateIR(node->children[i], level + 1, builder, module, context);
    }
}

void generateBitcodeAndObjectFile(LLVMModuleRef module) {
    if (LLVMWriteBitcodeToFile(module, "output.bc") != 0) {
        fprintf(stderr, "Error: Could not write bitcode to output.bc\n");
        return;
    }

    int result = system("llc output.bc -filetype=obj -o output.o");
    if (result != 0) {
        fprintf(stderr, "Error: Could not generate object file from bitcode\n");
        return;
    }
    printf("Successfully generated output.bc and output.o\n");
}

void generateLLVM(){
    LLVMPrintModuleToFile(module, "output.ll", NULL);
}

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

    initializeLLVM();

    if (yyparse() == 0) {
        printf("Parsing completed successfully.\n");

        if (program == NULL) {
            printf("AST is empty.\n");
        } else {
            printf("Generated AST:\n");
            printAST(program, 0);
        }
        printf("\n");
        generateIR(program, 0, builder, module, context);
        finalizeIR();
        printSymbolTable();
    } else {
        printf("Parsing failed.\n");
    }

    printf("\nTokens:\n");
    for (int i = 0; i < TOKEN_COUNT; i++) {
        printf("Token: %s, Type: %s\n", tokens[i].token, tokens[i].type);
    }
    printf("\n");

    free(tokens);
    fclose(yyin);

    generateBitcodeAndObjectFile(module);

    return 0;
}
