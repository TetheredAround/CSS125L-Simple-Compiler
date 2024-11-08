# Instructions
---
# Linux Instructions
Using Ubuntu 22.04, the following is verified to be working.
## Prequisites:
- Clang 
- LLVM
- Flex
- Bison
- Make
- GCC
## Installation of Prerequisites:
- ```sudo apt-get update```
- ```sudo apt-get install clang llvm flex bison make gcc```
## Installation of Compiler:
- ```make```
## How to Use:
- ```./simple_compiler [file]```

# Windows Instructions
Currently does **NOT** work due to library header issues, LLVM must be installed via Visual Studio Code and must be built from source.
### Use Makefile:
- ```make```
- ```.\simple_compiler [file].cpp```
## To Install Make:
#### Install make via Chocolatey
- install [Chocolatey](https://chocolatey.org/install).
    - Choose individual.
    - Follow instructions.

Alternatively:

- Open Powershell as administrator.
- Enter the following command: ```Set-ExecutionPolicy AllSigned```
- Then install Chocolatey using the following command: ```Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))```

After installing, run the following command:
```choco install make```.

Then restart terminal and test using ```make```.

---
## To Implement:
- ~~Lexical Analysis~~
    - Tokenization of C++ code to identify keywords, operators, identifiers, and
literals. 
- ~~Syntax Analysis~~
    - Parsing the token stream to construct an Abstract Syntax Tree (AST) based on a
defined grammar.
- ~~Semantic Analysis~~
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

# Independent Components
The following are no longer viable to test the individual components as they are now dependent on each other.
#### Compile and Test Lexer:
- ```flex {lexer.l}```
- ```gcc lex.yy.c```
- ```Use the following to run:```
    - ```./a```
    - ```a.exe```
#### Compile and Test Parser:
- ```bison -d {parser.y}```
- ...
---