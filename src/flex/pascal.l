%{
#include "node/ModuleNode.hpp"
#include "node/DeclNode.hpp"
#include "node/ExpNode.hpp"
#include "node/StmtNode.hpp"
#include "pascal.tab.h"
#include <string.h>
#include <stdio.h>
int lineno = 0;
extern YYSTYPE yylval;
extern void yyerror(char const *s);
%}
%option noyywrap
%option yylineno
%option never-interactive
%option case-insensitive

letter 		[a-zA-Z]
digit  		[0-9]
hexdigit	[0-9a-fA-F]
underline	_
newline		\n
whitespace	[ ]
char        [^\n\t \r]
%%
{newline}	{++lineno;}
"program"	{return PROGRAM;}
"procedure"     {return PROCEDURE;}
"."			{return DOT;}
";"			{return SEMI;}
","			{return COMMA;}
"="			{return EQUAL;}
"Const"		{return CONST;}
"Array"		{return ARRAY;}
"Type"		{return TYPE;}
"["			{return LB;}
"]"			{return RB;}
"of"		{return OF;}
"Record"	{return RECORD;}
"end"		{return END;}
":"			{return COLON;}
"(" 		{return LP;}
")"			{return RP;}
".."		{return DOTDOT;}
"-"			{return MINUS;}
"Var"		{return VAR;}
"Function"	{return FUNCTION;}
"Not"		{return NOT;}
">="		{return GE;}
">"			{return GT;}
"<="		{return LE;}
"<"			{return LT;}
"+"			{return PLUS;}
"*"			{return MUL;}
"/"			{return DIV;}
"and"		{return AND;}
"mod"		{return MOD;}
"<>"		{return UNEQUAL;}
"or"		{return OR;}
":="		{return ASSIGN;}
"begin"		{return P_BEGIN;}
"if"		{return IF;}
"else"		{return ELSE;}
"then"		{return THEN;}
"repeat"	{return REPEAT;}
"until"		{return UNTIL;}
"while"		{return WHILE;}
"do"        {return DO;}
"for"       {return FOR;}
"Goto"		{return GOTO;}
"Case"		{return CASE;}
"to"		{return TO;}
"downto"	{return DOWNTO;}
"read"		{return READ;}
"write"     {return WRITE;}
"writeln"   {return WRITELN;}
"Integer"	{return TYPEINTEGER;}
"real"	    {return TYPEREAL;}
"char"      {return TYPECHAR;}
"string"    {return TYPESTRING;}
"bool"		{return TYPEBOOL;}
"false"		{
    yylval.constBool = false; 
    return P_FALSE;
}
"true"		{
    yylval.constBool = true;
    return P_TRUE;
}

"(*"|"{" {
    int c;
    while ((c = yyinput()) != '\0'){
        if (c == '}')
            break;
        else if (c == '*'){
            if ((c = yyinput()) == ')')
                break;
            else
                unput (c);
        }
        else if (c == '\n')
            ++lineno;
        else if (c == '\0')
            yyerror("unexpected EOF inside comment");
     }
}

\"(\\.|[^\\"])*\" {
//allow escape but not allow " and single "\"
    return STRING;
}

'(\\.|[^\\'])+' {
    char ch = yytext[1];
    if(ch == '\\' && strlen(yytext) > 3){ 
      switch(yytext[2]){
        case 'n':
            ch = '\n'; break;
        case 'r':
            ch = '\r'; break;
        case '\'':
            ch = '\''; break;
        case '\"':
            ch = '\"'; break;
      }
    }
    yylval.constChar = ch; 
	return CHAR;
}

[[:digit:]]+        {
  yylval.constInt = atoi(yytext);
  return INTEGER;
}
[[:digit:]]+'.'[[:digit:]]+ {
  yylval.constDouble = atof(yytext);
  return REAL;
}
[[:alpha:]_][[:alnum:]_]*  {
  return IDENTIFIER;
}

%%

