#ifndef SYMB_H
#define SYMB_H


// Symbol Table Structures
typedef struct Symbol {
    char* name;
    char* type;
    int initialized; // 1 if initialized, 0 otherwise
} Symbol;

typedef struct SymbolTable {
    Symbol* symbols;
    int count;
    int size;
} SymbolTable;

// Symbol Table functions
void initSymbolTable();
int addSymbol(const char *name, const char *type);
void setInitialized(const char *name);
int isInitialized(const char *name);
const char* getType(const char *name);
void printSymbolTable();

#endif