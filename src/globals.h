#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Yacc/Bison generates internally its own values
 * for the tokens. Other files can access these values
 * by including the tab.h file generated using the
 * Yacc/Bison option -d ("generate header")
 *
 * The YYPARSER flag prevents inclusion of the tab.h
 * into the Yacc/Bison output itself
 */

#ifndef YYPARSER

// Node
#include "node/TreeNode.hpp"
#include "node/ExpNode.hpp"
#include "node/ModuleNode.hpp"
#include "node/DeclNode.hpp"
#include "node/StmtNode.hpp"
// Symbol table
#include "symtab/symboltable.h"
// Code generation
#include "codegen/codegen.hpp"

/* the name of the following file may change */
#include "pascal.tab.h"

/* ENDFILE is implicitly defined by Yacc/Bison,
 * and not included in the tab.h file
 */
#define ENDFILE 0
#endif
 
extern FILE* source; /* source code text file */
extern FILE* listing;
// extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

#endif
