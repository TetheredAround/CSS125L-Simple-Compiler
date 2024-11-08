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

int is_integer_type(LLVMTypeRef type) {
    return LLVMGetTypeKind(type) == LLVMIntegerTypeKind;
}

int is_string_type(LLVMTypeRef type) {
    return LLVMGetTypeKind(type) == LLVMPointerTypeKind &&
           LLVMGetElementType(type) == LLVMInt8Type();
}

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

    LLVMTypeRef printfParamTypes[] = {LLVMPointerType(LLVMInt8Type(), 0)};
    LLVMTypeRef printfType = LLVMFunctionType(LLVMInt32Type(), printfParamTypes, 1, 1);
    LLVMAddFunction(module, "printf", printfType);

    LLVMTypeRef scanfParamTypes[] = {LLVMPointerType(LLVMInt8Type(), 0)};
    LLVMTypeRef scanfType = LLVMFunctionType(LLVMInt32Type(), scanfParamTypes, 1, 1);
    LLVMAddFunction(module, "scanf", scanfType);
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

    if (strcmp(node->type, "initialization") == 0) {
        printf("Initializing global variable %s...\n", node->value);

        LLVMTypeRef intType = LLVMInt32TypeInContext(context);

        LLVMValueRef globalVar = LLVMAddGlobal(module, intType, node->value);
        LLVMSetLinkage(globalVar, LLVMExternalLinkage);
        LLVMSetVisibility(globalVar, LLVMDefaultVisibility);

        LLVMValueRef defaultValue = LLVMConstInt(intType, 0, 0);
        LLVMSetInitializer(globalVar, defaultValue);

        for (size_t i = 0; i < node->num_children; ++i) {
            ASTNode* child = node->children[i];
            if (strcmp(child->type, "integer") == 0 && child->value) {
                int initValue = atoi(child->value);
                LLVMValueRef constValue = LLVMConstInt(intType, initValue, 0);
                LLVMSetInitializer(globalVar, constValue);
            }
        }
    }
    else if (strcmp(node->type, "output") == 0) {
        printf("Output of variable %s...\n", node->value);

        if (node->num_children == 0) {
            LLVMValueRef varAlloc = LLVMGetNamedGlobal(module, node->value);
            if (varAlloc == NULL) {
                fprintf(stderr, "Error: Variable '%s' not found in global scope.\n", node->value);
                return;
            }

            LLVMTypeRef intType = LLVMInt32Type();

            LLVMValueRef loadedValue = LLVMBuildLoad2(builder, intType, varAlloc, "loadedValue");

            LLVMTypeRef printfParamTypes[] = {LLVMPointerType(LLVMInt8Type(), 0)};
            LLVMTypeRef printfType = LLVMFunctionType(LLVMInt32Type(), printfParamTypes, 1, 1);

            LLVMValueRef printfFunc = LLVMGetNamedFunction(module, "printf");
            if (printfFunc == NULL) {
                printfFunc = LLVMAddFunction(module, "printf", printfType);
            }

            LLVMValueRef formatStr = LLVMBuildGlobalString(builder, "%d\n", "fmt");
            LLVMValueRef printfArgs[] = {formatStr, loadedValue};
            LLVMBuildCall2(builder, printfType, printfFunc, printfArgs, 2, "");
        }
    }
    else if (strcmp(node->type, "input") == 0) {
        LLVMTypeRef scanfParamTypes[] = {LLVMPointerType(LLVMInt8Type(), 0)};
        LLVMTypeRef scanfType = LLVMFunctionType(LLVMInt32Type(), scanfParamTypes, 1, 1);

        LLVMValueRef scanfFunc = LLVMGetNamedFunction(module, "scanf");
        if (scanfFunc == NULL) {
            scanfFunc = LLVMAddFunction(module, "scanf", scanfType);
        }

        LLVMValueRef varAlloc = LLVMGetNamedGlobal(module, node->value);
        if (varAlloc == NULL) {
            varAlloc = LLVMAddGlobal(module, LLVMInt32Type(), node->value);
            LLVMSetInitializer(varAlloc, LLVMConstInt(LLVMInt32Type(), 0, 0));
        }

        LLVMValueRef formatStr = LLVMBuildGlobalString(builder, "%d", "fmt");

        LLVMValueRef scanfArgs[] = {formatStr, varAlloc};
        LLVMBuildCall2(builder, scanfType, scanfFunc, scanfArgs, 2, "");
    }

    /*else if (strcmp(node->type, "declaration") == 0) {
    } else if (strcmp(node->type, "output") == 0) {
    } else if (strcmp(node->type, "input") == 0) {
    } */

    for (size_t i = 0; i < node->num_children; ++i) {
        generateIR(node->children[i], level + 1, builder, module, context);
    }
}

void generateBitcodeAndObjectFile(LLVMModuleRef module) {
    if (LLVMWriteBitcodeToFile(module, "output.bc") != 0) {
        fprintf(stderr, "Error: Could not write bitcode to output.bc\n");
        return;
    }

    int result = system("llc -relocation-model=pic output.bc -filetype=obj -o output.o");
    if (result != 0) {
        fprintf(stderr, "Error: Could not generate object file from bitcode\n");
        return;
    }
    printf("Successfully generated output.bc and output.o with PIC\n");

    result = system("llc output.ll -o output.asm");
    if (result != 0) {
        fprintf(stderr, "Error: Could not generate object file from assembly\n");
        return;
    }

    printf("Successfully generated assembly file\n");

    result = system("gcc -fPIE -pie output.o -o program_executable");
    if (result != 0) {
        fprintf(stderr, "Error: Could not link object file to create executable\n");
        return;
    }
    printf("Successfully generated executable: program_executable\n");
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
