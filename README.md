# Instructions
---
## How to Use Standalone All-in-One Compiler:
### Compile Lexer, if uncompiled:
- ```flex {lexer.l}```
### Compile Parser, if uncompiled:
- ...
### Compile the Compiler:
- ```gcc lex.yy.c main.c -o simple_compiler ```
- ```.\simple_compiler [file]```
## Sample Code
- ```.\ simple_compiler toLex.c```

---
# Standalone Components
### Compile and Test Lexer:
- ```flex {lexer.l}```
- ```gcc lex.yy.c```
- ```Use the following to run:```
    - ```./a```
    - ```a.exe```
### Compile and Test Parser:
...

---
## To Implement:
- ~~Lexical Analysis~~
    - Tokenization of C++ code to identify keywords, operators, identifiers, and
literals. 
- Syntax Analysis
    - Parsing the token stream to construct an Abstract Syntax Tree (AST) based on a
defined grammar.
- Semantic Analysis
    - Checking for semantic errors, such as type mismatches and variable scope
resolution. 
- Intermediate Code Generation
    - Translating the AST into an intermediate representation (IR)
that is easier to manipulate.
- Code Generation
    - Producing assembly or machine code from the intermediate representation. 

## Optional
- Code Optimization
    - Basic optimization techniques to improve the efficiency of the generated
code.   
- Error Handling
    - Providing informative error messages for lexical, syntax, and semantic errors. 