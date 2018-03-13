%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include <cstdio>
#include <string>
#include <cstring>
#include "node/ModuleNode.hpp"
#include "node/DeclNode.hpp"
#include "node/ExpNode.hpp"
#include "node/StmtNode.hpp"

/* stores syntax tree for later return */
static TreeNode * savedTree = nullptr;

extern int lineno;
extern int yylineno;
extern int yylex(void);
extern void yyerror(const char *message);
extern char* yytext;
%}
%debug
%define parse.error verbose

%union {
    TreeNode* tree_node;
    Id_Node*  id_node;
    Program_Node* program_node; 
    Program_head_Node* program_head_node;
    Routine_Node* routine_node;
    Routine_head_Node* routine_head_node;

    Const_part_Node* const_part_node;
    Const_expr_list_Node* const_expr_list_node;
    Const_value_Node* const_value_node;
    int     constInt;
    double  constDouble;
    char    constChar;
    bool    constBool;
    Type_part_Node*         type_part_node;
    Type_decl_list_Node*    type_decl_list_node;
    Type_definition_Node*   type_definition_node;
    Type_decl_Node*         type_decl_node;
    Array_type_decl_Node*   array_type_decl_node;
    Record_type_decl_Node*  record_type_decl_node;
    Field_decl_list_Node*   field_decl_list_node;
    Field_decl_Node*        field_decl_node;
    Simple_type_decl_Node*  simple_type_decl_node;
    Name_list_Node*         name_list_node;
    Var_part_Node*          var_part_node;
    Var_decl_list_Node*     var_decl_list_node;
    Var_decl_Node*          var_decl_node;
    Routine_part_Node*      routine_part_node;
    Function_decl_Node*     function_decl_node;
    Procedure_decl_Node*    procedure_decl_node;
    Parameters_Node*        parameters_node;
    Para_decl_list_Node*    para_decl_list_node;
    Para_type_list_Node*    para_type_list_node;
    Var_para_list_Node*     var_para_list_node;
    Val_para_list_Node*     val_para_list_node;
    Routine_body_Node*      routine_body_node;
    Compound_stmt_Node*     compound_stmt_node;
    Stmt_list_Node*         stmt_list_node;
    Stmt_Node*              stmt_node;
    Non_label_stmt_Node*    non_label_stmt_node;
    Assign_stmt_Node*       assign_stmt_node;
    Proc_stmt_Node*         proc_stmt_node;
    If_stmt_Node*           if_stmt_node;
    Else_clause_Node*       else_clause_node;
    Repeat_stmt_Node*       repeat_stmt_node;
    While_stmt_Node*        while_stmt_node;
    For_stmt_Node*          for_stmt_node;
    Direction_Node*         direction_node;
    Case_stmt_Node*         case_stmt_node;
    Case_expr_list_Node*    case_expr_list_node;
    Case_expr_Node*         case_expr_node;
    Goto_stmt_Node*         goto_stmt_node;
    Expression_list_Node*   expression_list_node;
    Expression_Node*        expression_node;
    Expr_Node*              expr_node;
    Factor_Node*            factor_node;
    Args_list_Node*         args_list_node;

}

%token  IDENTIFIER
%token<constInt>  INTEGER
%token<constDouble>  REAL
%token<constChar>  CHAR
%token<constBool>   P_TRUE;
%token<constBool>   P_FALSE;
%token  STRING
%token PROGRAM DOT SEMI COMMA EQUAL CONST ARRAY TYPE LB RB OF RECORD END COLON LP RP DOTDOT MINUS VAR FUNCTION NOT GE GT LE LT
%token PLUS MUL DIV AND MOD UNEQUAL OR ASSIGN P_BEGIN IF ELSE THEN REPEAT UNTIL WHILE DO FOR GOTO CASE TO DOWNTO READ WRITE WRITELN
%token TYPEINTEGER TYPEREAL TYPECHAR TYPESTRING TYPEBOOL PROCEDURE  

%start program

%type<id_node>              id;
%type<program_node>         program;
%type<program_head_node>    program_head;
%type<routine_node>         routine; 
%type<routine_head_node>    routine_head;
%type<const_part_node>      const_part;
%type<const_expr_list_node> const_expr_list;
%type<const_value_node>     const_value;
%type<type_part_node>       type_part;
%type<type_decl_list_node>  type_decl_list;
%type<type_definition_node> type_definition;
%type<type_decl_node>       type_decl;
%type<array_type_decl_node> array_type_decl;
%type<record_type_decl_node> record_type_decl;
%type<field_decl_list_node> field_decl_list;
%type<field_decl_node>      field_decl;
%type<simple_type_decl_node> simple_type_decl;
%type<name_list_node>       name_list;
%type<var_part_node>        var_part;
%type<var_decl_list_node>   var_decl_list;
%type<var_decl_node>        var_decl;
%type<routine_part_node>    routine_part;
%type<function_decl_node>   function_decl;
%type<procedure_decl_node>  procedure_decl;
%type<parameters_node>      parameters;
%type<para_decl_list_node>  para_decl_list;
%type<para_type_list_node>  para_type_list;
%type<var_para_list_node>   var_para_list;
%type<val_para_list_node>   val_para_list;
%type<routine_body_node>    routine_body;
%type<compound_stmt_node>   compound_stmt;
%type<stmt_list_node>       stmt_list;
%type<stmt_node>            stmt;
%type<non_label_stmt_node>  non_label_stmt;
%type<assign_stmt_node>     assign_stmt;
%type<proc_stmt_node>       proc_stmt;
%type<if_stmt_node>         if_stmt;
%type<else_clause_node>     else_clause;
%type<repeat_stmt_node>     repeat_stmt;
%type<while_stmt_node>      while_stmt;
%type<for_stmt_node>        for_stmt;
%type<direction_node>       direction;
%type<case_stmt_node>       case_stmt;
%type<case_expr_list_node>  case_expr_list;
%type<case_expr_node>       case_expr;
%type<goto_stmt_node>       goto_stmt;
%type<expression_list_node> expression_list;
%type<expression_node>      expression;
%type<expr_node>            expr;
%type<expr_node>            term;
%type<factor_node>          factor;
%type<args_list_node>       args_list;

%%

id : IDENTIFIER{
    $$ = new Id_Node(yytext);
}

program : program_head  routine  DOT{
              $$ = new Program_Node($1, $2);
              $$->setLineno(lineno);
              savedTree = $$;
          };

program_head  : PROGRAM  id SEMI{
                    $$ = new Program_head_Node($2);
                };

routine : routine_head  routine_body{ 
              $$ = new Routine_Node($1, $2);
              $$->setLineno(lineno);
          }       |{//empty
              $$ = nullptr;
          };

routine_head : const_part  type_part  var_part  routine_part{
                   $$ = new Routine_head_Node($1, $2, $3, $4);
                   $$->setLineno(lineno);
               };

const_part : CONST  const_expr_list  {
                 $$ = new Const_part_Node($2);
             }
|{$$ = nullptr;}
;

const_expr_list : const_expr_list  id EQUAL const_value SEMI{
                      $$ = new Const_expr_list_Node($1, $2, $4);
                      $$->setLineno(lineno);
                  }
| id EQUAL  const_value  SEMI{ 
    $$ = new Const_expr_list_Node($1, $3);
    $$->setLineno(lineno);
};

const_value : INTEGER {
                  /* const_value can be ConstInt_Node or else
                     but all is Const_value_Node
                   */
                  $$ = new ConstInt_Node($1);
                  $$->setLineno(lineno);
              }
|  REAL{ 
    $$ = new ConstDouble_Node($1);
    $$->setLineno(lineno);
} 
|  CHAR{
    $$ = new ConstChar_Node($1);
    $$->setLineno(lineno);
}
|  STRING{
    $$ = new ConstStr_Node(std::string(yytext + 1, strlen(yytext) - 2));
    //strip ""
    $$->setLineno(lineno);
}
|   P_TRUE{
    $$ = new ConstBool_Node($1);
    $$->setLineno(lineno);
} | P_FALSE{
    $$ = new ConstBool_Node($1);
    $$->setLineno(lineno);
};

type_part: TYPE type_decl_list {
               $$ = new Type_part_Node($2);
               $$->setLineno(lineno);
           }         
| {$$ = nullptr;};

type_decl_list  : type_decl_list  type_definition {
                      $$ = new Type_decl_list_Node($1, $2);
                      $$->setLineno(lineno);
                  }
|  type_definition {
    $$ = new Type_decl_list_Node($1);
    $$->setLineno(lineno);
};

type_definition : id EQUAL  type_decl  SEMI{ 
                      $$ = new Type_definition_Node($1, $3);
                      $$->setLineno(lineno);  
                  };

type_decl    : simple_type_decl {$$ = $1;}  
|  array_type_decl  {$$ = $1;}
|  record_type_decl {$$ = $1;};

array_type_decl : ARRAY  LB  simple_type_decl  RB  OF  type_decl{ 
                      $$ = new Array_type_decl_Node($3, $6);
                      $$->setLineno(lineno);
                  };

record_type_decl : RECORD  field_decl_list  END{
                       $$ = new Record_type_decl_Node($2);
                       $$->setLineno(lineno);
                   };

field_decl_list : field_decl_list  field_decl{
                      $$ = new Field_decl_list_Node($1, $2);
                      $$->setLineno(lineno);
                  }               
|  field_decl{
    $$ = new Field_decl_list_Node($1);
    $$->setLineno(lineno);
};

field_decl : name_list  COLON  type_decl  SEMI{
                 $$ = new Field_decl_Node($1, $3);
                 $$->setLineno(lineno);
             };

simple_type_decl : TYPEINTEGER{
                       $$ = new System_type_decl_Node(System_type_decl_Node::INT);
                       $$->setLineno(lineno);
                   }  
|  TYPEREAL{
    $$ = new System_type_decl_Node(System_type_decl_Node::REAL);
    $$->setLineno(lineno);
}
|  TYPECHAR{
    $$ = new System_type_decl_Node(System_type_decl_Node::CHAR);
    $$->setLineno(lineno);
}
|  TYPESTRING{
    $$ = new System_type_decl_Node(System_type_decl_Node::STRING);
    $$->setLineno(lineno);
}
|  TYPEBOOL {
    $$ = new System_type_decl_Node(System_type_decl_Node::BOOL);
    $$->setLineno(lineno);
}
|  id  {
    $$ = new Alias_type_decl_Node($1);
    $$->setLineno(lineno);
}
|  LP  name_list  RP{
    $$ = new Enum_type_decl_Node($2);
    $$->setLineno(lineno);
}
|  const_value  DOTDOT  const_value{
    $$ = new Subrange_const_value_type_decl_Node(false, $1, false, $3);
    $$->setLineno(lineno);
}  
|  MINUS  const_value  DOTDOT  const_value
{
    $$ = new Subrange_const_value_type_decl_Node(true, $2, false, $4);
    $$->setLineno(lineno);
} 
|  MINUS  const_value  DOTDOT  MINUS  const_value
{
    $$ = new Subrange_const_value_type_decl_Node(true, $2, true, $5);
    $$->setLineno(lineno);
} |  id DOTDOT  id{
    $$ = new Subrange_id_type_decl_Node($1, $3);
    $$->setLineno(lineno);
};

name_list : name_list  COMMA  id {
                $$ = new Name_list_Node($1, $3);
                $$->setLineno(lineno);
            }   |  id { 
                $$ = new Name_list_Node($1);
                $$->setLineno(lineno);
            };

var_part : VAR  var_decl_list {
               $$ = new Var_part_Node($2);
               $$->setLineno(lineno);
           } | { $$ = nullptr;
           };

var_decl_list : var_decl_list  var_decl{
                    $$ = new Var_decl_list_Node($1, $2);
                }  
|  var_decl{ $$ = new Var_decl_list_Node($1);};

var_decl : name_list  COLON  type_decl  SEMI{
               $$ = new Var_decl_Node($1, $3);
               $$->setLineno(lineno);
           };

routine_part : routine_part  function_decl {
                   $$ = new Routine_part_Node($1, (Function_decl_Node*)$2);
                   $$->setLineno(lineno);
               }   
|  routine_part  procedure_decl{
    $$ = new Routine_part_Node($1, (Procedure_decl_Node*)$2);
    $$->setLineno(lineno);
}  
|  function_decl {
    $$ = new Routine_part_Node((Function_decl_Node*)$1);
    $$->setLineno(lineno);
} 
|  procedure_decl{
    $$ = new Routine_part_Node((Procedure_decl_Node*)$1);
    $$->setLineno(lineno);
}
| {$$ = nullptr;};

function_decl : FUNCTION  id parameters  COLON  simple_type_decl SEMI routine SEMI{
                    $$ = new Function_decl_Node($2, $3, $5, $7);
                    $$->setLineno(lineno);
                };

procedure_decl :PROCEDURE id parameters  SEMI  routine  SEMI{ 
                    $$ = new Procedure_decl_Node($2, $3, $5);
                    $$->setLineno(lineno);
                };

parameters : LP  para_decl_list  RP {
                 $$ = new Parameters_Node($2);
                 $$ -> setLineno(lineno);
             }
| {$$ = nullptr;}
| LP RP{$$ = nullptr;};

para_decl_list : para_decl_list  SEMI  para_type_list{
                     $$ = new Para_decl_list_Node($1, $3);
                     $$->setLineno(lineno);
                 }
| para_type_list{
    $$ = new Para_decl_list_Node($1);
    $$->setLineno(lineno);
}; 

para_type_list : var_para_list COLON  simple_type_decl{ 
                     $$ = new Para_type_list_Node($1, $3);
                     $$->setLineno(lineno);
                } 
| val_para_list  COLON  simple_type_decl{ 
    $$ = new Para_type_list_Node($1, $3);
    $$->setLineno(lineno);
}; 

var_para_list : VAR  name_list{
                    $$ = new Var_para_list_Node($2);
                    $$->setLineno(lineno);
                };

val_para_list : name_list {
                    $$ = new Val_para_list_Node($1);
                    $$->setLineno(lineno);
                };

routine_body : compound_stmt {
                   $$ = new Routine_body_Node($1);
                   $$->setLineno(lineno);
               };

compound_stmt : P_BEGIN  stmt_list  END {
                    $$ = new Compound_stmt_Node($2);
                    $$->setLineno(lineno);
                };

stmt_list : stmt_list  stmt  SEMI { 
                $$ = new Stmt_list_Node($1, $2);
                $$->setLineno(lineno);  
            } | {$$ = nullptr;};

stmt : INTEGER  COLON  non_label_stmt{ 
           $$ = new Stmt_Node($1, $3);
           $$->setLineno(lineno); 
       } |  non_label_stmt{
           $$ = new Stmt_Node($1);
           $$->setLineno(lineno);
       };

non_label_stmt : assign_stmt{$$ = $1;}
| proc_stmt {$$ = $1;}
| compound_stmt{$$ = $1;} 
| if_stmt {$$ = $1;}
| repeat_stmt  {$$ = $1;}
| while_stmt  {$$ = $1;}
| for_stmt {$$ = $1;}
| case_stmt  {$$ = $1;}
| goto_stmt  {$$ = $1;}
| error {$$ = nullptr;};

assign_stmt : id ASSIGN  expression{ 
                  $$ = new Assign_id_stmt_Node($1, $3);
                  $$->setLineno(lineno);
              }
| id LB expression RB ASSIGN expression{
    $$ = new Assign_arr_stmt_Node($1, $3, $6);
    $$->setLineno(lineno);
}
| id DOT id ASSIGN  expression{ 
    $$ = new Assign_record_stmt_Node($1, $3, $5);
    $$->setLineno(lineno);
};

proc_stmt : id LP RP{
                $$ = new Proc_stmt_Node($1);
                $$->setLineno(lineno);
            }
|  id LP  args_list  RP {
    $$ = new Proc_stmt_Node($1, $3);
    $$->setLineno(lineno);
}
|  READ  LP  id  RP {
    $$ = new Read_stmt_Node($3);
    $$->setLineno(lineno);
}
| WRITE LP expression RP{
    $$ = new Write_stmt_Node($3);
    $$->setLineno(lineno);
}
| WRITELN LP expression RP{
    $$ = new Writeln_stmt_Node($3);
    $$->setLineno(lineno);
};

if_stmt : IF  expression  THEN  stmt  else_clause{ 
              $$ = new If_stmt_Node($2, $4, $5);
              $$->setLineno(lineno);
          };

else_clause : ELSE stmt {
                  $$ = new Else_clause_Node($2);
                  $$->setLineno(lineno);
              }
| {
    $$ = nullptr;
};

repeat_stmt : REPEAT  stmt_list  UNTIL  expression{ 
                  $$ = new Repeat_stmt_Node($2, $4);
                  $$->setLineno(lineno);
              };

while_stmt : WHILE  expression  DO stmt{
                 $$ = new While_stmt_Node($2, $4);
                 $$->setLineno(lineno);
             };

for_stmt : FOR  id ASSIGN  expression  direction  expression  DO stmt{ 
               $$ = new For_stmt_Node($2, $4, $5, $6, $8);
               $$->setLineno(lineno);
           };

direction : TO {
                $$ = new Direction_Node(true);
                $$->setLineno(lineno);
            }
| DOWNTO{
    $$ = new Direction_Node(false);
    $$->setLineno(lineno);
};

case_stmt : CASE expression OF case_expr_list  END{ 
                $$ = new Case_stmt_Node($2, $4);
                $$->setLineno(lineno);
            };

case_expr_list : case_expr_list  case_expr{ 
                     $$ = new Case_expr_list_Node($1, $2);
                 }
| case_expr{
    $$ = new Case_expr_list_Node($1);
};

case_expr : const_value  COLON  stmt  SEMI{ 
                $$ = new Case_const_val_expr_Node($1, $3);
                $$->setLineno(lineno);
            }
|  id COLON  stmt  SEMI{
    $$ = new Case_id_expr_Node($1, $3);
    $$->setLineno(lineno);
};

goto_stmt : GOTO  INTEGER{ 
                $$ = new Goto_stmt_Node($2);
                $$->setLineno(lineno);
            };

expression_list : expression_list  COMMA  expression {
                      $$ = new Expression_list_Node($1, $3);
                      $$->setLineno(lineno);
                  }
| expression {
    $$ = new Expression_list_Node($1);
    $$->setLineno(lineno);
};

expression : expression  GE  expr{
                 $$ = new Expression_Node($1, Expression_Node::GE, $3);
                 $$->setLineno(lineno);
             }
|  expression  GT  expr { 
    $$ = new Expression_Node($1, Expression_Node::GT, $3);
    $$->setLineno(lineno);
} 
|  expression  LE  expr{ 
    $$ = new Expression_Node($1, Expression_Node::LE, $3);
    $$->setLineno(lineno);
}
|  expression  LT  expr { 
    $$ = new Expression_Node($1, Expression_Node::LT, $3);
    $$->setLineno(lineno);
}
|  expression  EQUAL  expr{
    $$ = new Expression_Node($1, Expression_Node::EQ, $3);
    $$->setLineno(lineno);
}
|  expression  UNEQUAL  expr  { 
    $$ = new Expression_Node($1, Expression_Node::NEQ, $3);
    $$->setLineno(lineno);
}
|  expr {
    $$ = new Expression_Node($1);
    $$->setLineno(lineno);
}
| error {
    $$ = nullptr;
};

expr : expr  PLUS  term  {
           $$ = new Expr_Node($1, Expr_Node::ADD, $3);
           $$->setLineno(lineno);
       }
| expr  MINUS  term {
    $$ = new Expr_Node($1, Expr_Node::SUB, $3);
    $$->setLineno(lineno);
}
| expr  OR  term {
    $$ = new Expr_Node($1, Expr_Node::OR, $3);
    $$->setLineno(lineno);
}
| term {
  $$ = $1;
}
| error{
  $$ = nullptr;
};

term : term  MUL  factor{
           $$ = new Expr_Node($1, Expr_Node::MUL, new Expr_Node($3));
           $$->setLineno(lineno);
       }
|  term  DIV  factor{ 
    $$ = new Expr_Node($1, Expr_Node::DIV, new Expr_Node($3));
    $$->setLineno(lineno);
}
|  term  MOD  factor{ 
    $$ = new Expr_Node($1, Expr_Node::MOD, new Expr_Node($3));
    $$->setLineno(lineno);
}
|  term  AND factor{
    $$ = new Expr_Node($1, Expr_Node::AND, new Expr_Node($3));
    $$->setLineno(lineno);
}
|  factor {
    $$ = new Expr_Node($1);
    $$->setLineno(lineno);
}
| error{
    $$ = nullptr;
};

factor  : id {
              $$ = new Factor_id_Node($1);
              $$->setLineno(lineno);
          } 
|  const_value{
    $$ = $1;
    //const_val is factor
}
|  LP  expression  RP{
    $$ = $2;
    //expression is factor
}
|  NOT  factor {
    $$ = new Factor_unary_Node(Factor_unary_Node::NOT, $2);
    $$->setLineno(lineno);
} 
|  MINUS  factor{
    $$ = new Factor_unary_Node(Factor_unary_Node::MINUS, $2);
    $$->setLineno(lineno);
}  
|  id LB  expression  RB{
    $$ = new Factor_arr_Node($1, $3);
    $$->setLineno(lineno);
}
|  id DOT  id{
    $$ = new Factor_record_Node($1, $3);
    $$->setLineno(lineno);
}
|   id LP args_list RP{
    $$ = new Func_call_Node($1, $3);
    $$->setLineno(lineno);
}
|   id LP RP{
    $$ = new Func_call_Node($1);
    $$->setLineno(lineno);
}
| error{$$ = nullptr;};

/*args_list is not null*/
args_list  :  args_list  COMMA  expression {
                  $$ = new Args_list_Node($1, $3);
                  $$->setLineno(lineno);
              }
|  expression{
    $$ = new Args_list_Node($1);
    $$->setLineno(lineno);
};

%%

void yyerror(char const *s){
       fprintf (stderr, "%s at %d\n", s, yylineno);
}

TreeNode * do_parse(void){
    auto res = yyparse();
    switch(res){
      case 0: 
        return savedTree;
        break;
      case 1:
        exit(0);
        break;
      case 2:
        fprintf(stderr, "%s\n", "Memory exhausted.");
        exit(0);
        break;
    }
}
