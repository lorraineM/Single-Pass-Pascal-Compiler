#ifndef _STMTNODE_HPP_
#define _STMTNODE_HPP_

#include "TreeNode.hpp"
#include "ExpNode.hpp"

#include "../symtab/symboltable.h"
#include "../codegen/codegen.hpp"

using namespace std;

class Non_label_stmt_Node;
class Stmt_Node; // Non_label_stmt_Node

class Assign_stmt_Node; // Expression_Node
class Assign_id_stmt_Node; // Id_Node
class Assign_arr_stmt_Node; // Id_Node, Expression_Node
class Assign_record_stmt_Node; // Id_Node

class Args_list_Node; // Args_list_Node, Expression_Node
class Proc_stmt_Node; // Id_Node, Args_list_Node
class Factor_Node;
class Read_stmt_Node; // Factor_Node
class Compound_stmt_Node; // Stmt_list_Node

class Else_clause_Node; // Stmt_Node
class If_stmt_Node; // Expression_Node, Stmt_Node, Else_clause_Node

class Repeat_stmt_Node; // Stmt_list_Node, Expression_Node
class While_stmt_Node; // Expression_Node, Stmt_Node
class Direction_Node;
class For_stmt_Node; // Id_Node, Expression_Node, Direction_Node, Expression_Node, Stmt_Node

class Case_expr_Node; // Stmt_Node
class Case_const_val_expr_Node; // Const_value_Node
class Case_id_expr_Node; // Id_Node
class Case_expr_list_Node; // Case_expr_list_Node, Case_expr_Node
class Case_stmt_Node; // Expression_Node, Case_expr_list_Node

class Goto_stmt_Node;
class Stmt_list_Node; // Stmt_list_Node, Stmt_Node




class Non_label_stmt_Node : public TreeNode{
public:
    virtual string build_symbol_table(string type){ return "";}
    virtual void gen_code(CodeGenerator* cg, int block_id) {}
    
protected:
    Non_label_stmt_Node(){}
};

class Stmt_Node : public TreeNode{
public:
    Stmt_Node(unsigned label, Non_label_stmt_Node * stmt)
        :label(label), hasLabel(true), stmt(stmt){}
    Stmt_Node(Non_label_stmt_Node * stmt)
        :label(0), hasLabel(false), stmt(stmt){}
    bool has_label() {return hasLabel;}
    string get_label() {return to_string(label) + ":";}
    bool get_hasLable() {return this->hasLabel;}
    void gen_code(CodeGenerator* cg, int block_id);
    string build_symbol_table(string type = "");

private:
    unsigned label;
    bool hasLabel;
    Non_label_stmt_Node * stmt;
};

class Assign_stmt_Node : public Non_label_stmt_Node{
protected:
    Assign_stmt_Node(Expression_Node *expr):expr(expr){}
    virtual void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);
protected:
    Expression_Node* expr;
};

class Assign_id_stmt_Node : public Assign_stmt_Node{
public:
    Assign_id_stmt_Node(Id_Node* id, 
        Expression_Node *expr)
        :Assign_stmt_Node(expr), id(id){}
    void gen_code(CodeGenerator* cg, int block_id) {};
    string build_symbol_table(string type);

private:
    Id_Node* id;
};

class Assign_arr_stmt_Node : public Assign_stmt_Node{
public:
    Assign_arr_stmt_Node(Id_Node* arr_name, 
        Expression_Node *index, Expression_Node *expr)
        :Assign_stmt_Node(expr), arr_name(arr_name), index(index){}
    void gen_code(CodeGenerator* cg, int block_id){}
    string build_symbol_table(string type);

private:
    Id_Node* arr_name;
    Expression_Node* index;
};

class Assign_record_stmt_Node : public Assign_stmt_Node{
public:
    Assign_record_stmt_Node(Id_Node* record_name, 
        Id_Node *member, Expression_Node *expr)
        :Assign_stmt_Node(expr), record_name(record_name), member(member){}
    void gen_code(CodeGenerator* cg, int block_id){}
    string build_symbol_table(string type);

private:
    Id_Node* record_name;
    Id_Node* member;
};

class Args_list_Node : public TreeNode{
public:
    Args_list_Node(Args_list_Node* prev, Expression_Node* expr)
        :prev(prev), expr(expr){}
    explicit Args_list_Node(Expression_Node* expr):prev(nullptr), expr(expr){}
    void gen_code(CodeGenerator* cg, int block_id){}
    string build_symbol_table(string type);
    
private:
    Args_list_Node *prev;
    Expression_Node *expr;
};


class Proc_stmt_Node : public Non_label_stmt_Node{
public:
    Proc_stmt_Node(Id_Node* id, Args_list_Node* args)
        :id(id), args(args){}
    Proc_stmt_Node(Id_Node* id)
        :id(id), args(nullptr){}
    void gen_code(CodeGenerator* cg, int block_id){}
    string build_symbol_table(string type);

private:
    Id_Node* id;
    Args_list_Node *args;
};

class Read_stmt_Node : public Proc_stmt_Node{
public:
    Read_stmt_Node(Id_Node* id)
        :Proc_stmt_Node(new Id_Node("read"), 
            new Args_list_Node(
                new Expression_Node(
                    new Expr_Node(
                        new Factor_id_Node(id))))), id(id){}
    void gen_code(CodeGenerator* cg, int block_id){}
    string build_symbol_table(string type);

private:
    Id_Node* id;//shortcut of args
};

class Write_stmt_Node : public Proc_stmt_Node{
public:
    Write_stmt_Node(Expression_Node* expression)
        :Proc_stmt_Node(new Id_Node("write"),
            new Args_list_Node(expression)), expression(expression){}
    void gen_code(CodeGenerator* cg, int block_id){}
    string build_symbol_table(string type);

private:
    Expression_Node* expression;//shortcut of args from Proc_stmt_Node
};

class Writeln_stmt_Node : public Proc_stmt_Node{
public:
    Writeln_stmt_Node(Expression_Node* expression)
        :Proc_stmt_Node(new Id_Node("write"),
            new Args_list_Node(expression)), expression(expression){}
    void gen_code(CodeGenerator* cg, int block_id);
    string build_symbol_table(string type = "");
    
private:
    Expression_Node* expression;//shortcut of args from Proc_stmt_Node
};



class Compound_stmt_Node : public Non_label_stmt_Node{
public:
    Compound_stmt_Node(Stmt_list_Node* stmts):stmts(stmts){}
    void gen_code(CodeGenerator* cg, int block_id);
    string build_symbol_table(string type = "");

private:
    Stmt_list_Node* stmts;
};

class Else_clause_Node : public Non_label_stmt_Node{
public:
    Else_clause_Node(Stmt_Node* stmt):stmt(stmt){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);

private:
    Stmt_Node* stmt;
};

class If_stmt_Node : public Non_label_stmt_Node{
public:
    If_stmt_Node(Expression_Node *exp, Stmt_Node* stmt, 
        Else_clause_Node* else_clause):exp(exp), stmt(stmt), else_clause(else_clause){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);
private:
    Expression_Node* exp;
    Stmt_Node* stmt;
    Else_clause_Node* else_clause;
};

class Repeat_stmt_Node : public Non_label_stmt_Node{
public:
    Repeat_stmt_Node(Stmt_list_Node* stmts, Expression_Node* expr)
        :stmts(stmts), expr(expr){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);

private:
    Stmt_list_Node* stmts;
    Expression_Node* expr;
};

class While_stmt_Node : public Non_label_stmt_Node{
public:
    While_stmt_Node(Expression_Node* expr, Stmt_Node* stmt)
        :expr(expr), stmt(stmt){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);
private:
    Expression_Node* expr;
    Stmt_Node* stmt;
};

class Direction_Node : public TreeNode{
public:
    Direction_Node(bool up):up(up){}
    void gen_code(CodeGenerator* cg, int block_id) {}
private:
    bool up;
};

class For_stmt_Node : public Non_label_stmt_Node{
public:
    For_stmt_Node(Id_Node* id, Expression_Node* id_exp,
        Direction_Node* direct, Expression_Node* exp, Stmt_Node *stmt)
        :id(id), id_exp(id_exp), direct(direct), exp(exp), stmt(stmt){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);

private:
    Id_Node *id;
    Expression_Node *id_exp;
    Direction_Node *direct;
    Expression_Node *exp;
    Stmt_Node *stmt;
};

class Case_expr_Node : public TreeNode{
public:
    string build_symbol_table(string type);
    void gen_code(CodeGenerator* cg, int block_id) {}
protected:
    Case_expr_Node(Stmt_Node* stmt):stmt(stmt){}
protected:
    Stmt_Node * stmt;
};

class Case_const_val_expr_Node : public Case_expr_Node{
public:
    Case_const_val_expr_Node(Const_value_Node* const_val, Stmt_Node* stmt)
        :Case_expr_Node(stmt), const_val(const_val){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);

private:
    Const_value_Node * const_val;
};

class Case_id_expr_Node : public Case_expr_Node{
public:
    Case_id_expr_Node(Id_Node* id, Stmt_Node* stmt)
        :Case_expr_Node(stmt), id(id){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);

private:
    Id_Node * id;
};


class Case_expr_list_Node : public TreeNode{
public:
    Case_expr_list_Node(Case_expr_list_Node *prev, Case_expr_Node *node)
        :prev(prev), node(node){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    explicit Case_expr_list_Node(Case_expr_Node *node):prev(nullptr), node(node){}
    string build_symbol_table(string type);

private:
    Case_expr_list_Node * prev;
    Case_expr_Node * node;
};

class Case_stmt_Node : public Non_label_stmt_Node{
public:
    Case_stmt_Node(Expression_Node *expr, Case_expr_list_Node *list)
        :expr(expr), list(list){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);
private:
    Expression_Node *expr;
    Case_expr_list_Node *list;
};

class Goto_stmt_Node : public Non_label_stmt_Node{
public:
    Goto_stmt_Node(int label): label(label){}
    void gen_code(CodeGenerator* cg, int block_id) {}
    string build_symbol_table(string type);
private:
    int label;
};

class Stmt_list_Node : public TreeNode{
public:
    Stmt_list_Node(Stmt_list_Node * prev, Stmt_Node * stmt)
        :prev(prev), stmt(stmt){}
    Stmt_list_Node(Stmt_Node * stmt):prev(nullptr), stmt(stmt){}
    void gen_code(CodeGenerator* cg, int block_id);
    string build_symbol_table(string type = "");
private:
    Stmt_list_Node * prev;
    Stmt_Node * stmt;
};

#endif
