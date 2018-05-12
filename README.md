# Single-Pass-Pascal-Compiler

#### It's a project for Compiler class.
#### Abstract: We used C++, yacc, lex to implement a single pass pascal compiler. The detailed syntax is listed on PASCAL_Syntax.pdf.
#### Code Organization:

* ./src
  * ./bison
    * pascal.y [yacc file] a scanner and a parser for grammar like constant expression, case statment and beginning of a separate compilation facility, generating a syntax tree finally. (There is no separate lex file since we combine it with yacc file.)
  * ./codegen
    * codegen.hpp, codegen.cpp, codegen_node.cpp 
    * a code generator, which generates MIPS file from input pascal source file.
  * ./node
    * DeclNode.hpp declaration node
    * ExpNode.hpp expression node
    * ModuleNode.hpp module node for variable, declaration, parameters, procedure, program, function, routine, expression...
    * StmtNode.hpp statement node
    * TreeNode.hpp syntax tree node
  * ./symtab
    * symboltable.h, symtab_build.cpp used for building symbol table
    * globals.h 
    * main.c main function source file parsing -> generate syntax tree -> build symbol table -> generate code -> output
    * Makefile

