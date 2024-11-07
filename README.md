# Instructions
---
## How to Use Standalone All-in-One Compiler:
### Use Makefile
- ```make```
- ```.\simple_compiler [file].cpp```
### Install make via Chocolatey
- install [Chocolatey](https://chocolatey.org/install).
    - Choose individual.
    - Follow instructions.
Alternatively:
- Open Powershell as administrator.
- Enter the following command: ```Set-ExecutionPolicy AllSigned```
- Then install Chocolatey using the following command: ```Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))```
After installing, run the following command:
- ```choco install make```
Then restart terminal and test using ```make```.
---
# To Update:
## Independent Components
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
## To Implement:
- ~~Lexical Analysis~~
    - Tokenization of C++ code to identify keywords, operators, identifiers, and
literals. 
- Syntax Analysis
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