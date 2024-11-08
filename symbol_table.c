#include "header/symbol_table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

SymbolTable symtab;

// Add a symbol to the table
int addSymbol(const char *name, const char *type) {
    printf("%s", name);
    for (int i = 0; i < symtab.count; ++i) {
        if (strcmp(symtab.symbols[i].name, name) == 0) {
            // Variable already exists
            return 0;
        }
    }
    if (symtab.count == symtab.size) {
        symtab.size = (symtab.size == 0) ? 10 : symtab.size * 2;
        symtab.symbols = realloc(symtab.symbols, symtab.size * sizeof(Symbol));
    }
    symtab.symbols[symtab.count].name = strdup(name);
    symtab.symbols[symtab.count].type = strdup(type);
    symtab.symbols[symtab.count].initialized = 0;
    ++symtab.count;
    return 1;
}

// Set a symbol as initialized
void setInitialized(const char *name) {
    for (int i = 0; i < symtab.count; ++i) {
        if (strcmp(symtab.symbols[i].name, name) == 0) {
            symtab.symbols[i].initialized = 1;
            return;
        }
    }
}

// Check if a symbol is initialized
int isInitialized(const char *name) {
    for (int i = 0; i < symtab.count; ++i) {
        if (strcmp(symtab.symbols[i].name, name) == 0) {
            return symtab.symbols[i].initialized;
        }
    }
    return 0;
}

// Get the type of a symbol
const char* getType(const char *name) {
    for (int i = 0; i < symtab.count; ++i) {
        if (strcmp(symtab.symbols[i].name, name) == 0) {
            return symtab.symbols[i].type;
        }
    }
    return NULL;
}

void printSymbolTable() {
    printf("Symbol Table:\n");
    printf("---------------------------------------------\n");
    printf("| %-12s | %-10s | %-12s |\n", "Variable Name", "Type", "Initialized");
    printf("---------------------------------------------\n");

    for (int i = 0; i < symtab.count; ++i) {
        const char* initialized = (symtab.symbols[i].initialized) ? "Yes" : "No";
        printf("| %-13s | %-10s | %-12s |\n", symtab.symbols[i].name, symtab.symbols[i].type, initialized);
    }
    printf("---------------------------------------------\n");
}