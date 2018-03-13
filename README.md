# Single-Pass-Pascal-Compiler

##### It's a project for Compiler class.
##### Abstract: paragraph we use C++, yacc, lex to implement a single pass pascal compiler. The detailed syntax is listed on PASCAL_Syntax.pdf.
##### Code Organization:

* ./src
  * ./bison
    * pascal.y [yacc file] a scanner and a parser for grammar like constant expression, case statment and beginning of a separate compilation facility, generating a syntax tree finally. (There is no separate lex file since we combine it with yacc file.)
  * ./codegen

