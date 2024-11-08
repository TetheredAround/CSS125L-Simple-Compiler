# Makefile for building and running the simple compiler

# Compiler and flags
CC = clang
CFLAGS = -g -Wall -Wno-unused-function `llvm-config --cflags --ldflags --libs core native`
LEX = flex
BISON = bison

# Source files
LEXER = lexer.l
PARSER = parser.y
MAIN = main.c
PARSER_TAB_C = parser.tab.c
PARSER_TAB_H = parser.tab.h
AST_C = ast.c
SYMBOL_TABLE_C = symbol_table.c
LEX_OUTPUT = lex.yy.c
EXEC = simple_compiler

# Targets
all: $(EXEC)

# Rule to build the lexer
$(LEX_OUTPUT): $(LEXER)
	$(LEX) $(LEXER)

# Rule to build the parser
$(PARSER_TAB_C) $(PARSER_TAB_H): $(PARSER)
	$(BISON) -d $(PARSER)

# Rule to compile the main and other source files
$(EXEC): $(LEX_OUTPUT) $(PARSER_TAB_C) $(MAIN) $(AST_C) $(SYMBOL_TABLE_C)
	$(CC) $(CFLAGS) $(LEX_OUTPUT) $(PARSER_TAB_C) $(MAIN) $(AST_C) $(SYMBOL_TABLE_C) -o $(EXEC)

# Rule to run the compiler
run: $(EXEC)
	./$(EXEC) test/PL1.cpp
	./$(EXEC) test/PL2.cpp
	./$(EXEC) test/PL3.cpp

# Clean rule to remove generated files
clean:
	rm -f $(LEX_OUTPUT) $(PARSER_TAB_C) $(PARSER_TAB_H) $(EXEC)

# Phony targets
.PHONY: all run clean
