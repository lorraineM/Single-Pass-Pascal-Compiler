#ifndef _EXPNODE_H_
#define _EXPNODE_H_

#include "TreeNode.hpp"
#include <sstream>
#include "../symtab/symboltable.h"
#include "../codegen/codegen.hpp"

using namespace std;

class Id_Node;
class Factor_Node;

class Const_value_Node;
class ConstInt_Node;
class ConstDouble_Node;
class ConstChar_Node;
class ConstBool_Node;
class ConstStr_Node;

class Factor_id_Node; // Id_Node
class Factor_unary_Node; // Factor_Node
class Args_list_Node; // from StmtNode
class Func_call_Node; // Id_Node, Args_list_Node

class Factor_arr_Node; // Id_Node, Expression_Node
class Factor_record_Node;

class Expr_Node; // Expr_Node, Factor_Node
class Expression_Node; // Expression_Node, Expr_Node
class Expression_list_Node; // Expression_list_Node, Expression_Node


class Id_Node : public TreeNode {
public:
	  Id_Node(const string& name) : name(name){}
	  string get_name() {return name;}
  
public:
    table_unit * sym_unit;
private:
  	string name;
};

class Factor_Node : public TreeNode{
public:
    virtual string build_symbol_table(string type){}
    virtual void gen_compute_code(CodeBlock* cb, string result_reg){}
protected:
    Factor_Node(){}
};

class Const_value_Node : public Factor_Node{
public:
    virtual string get_val(){};
    virtual int get_value(){};
    virtual string get_type(){};
    string build_symbol_table(string type);
    virtual void gen_compute_code(CodeBlock* cb, string result_reg){}
    
protected:
    Const_value_Node(){}
};

class ConstInt_Node : public Const_value_Node{
public:
    ConstInt_Node(int val): val(val){type = ".word";}
    string get_val() { return to_string(val); }
    int get_value() { return val; }
    string get_type() {return type;}
    void gen_data(CodeGenerator* cg);
    void gen_compute_code(CodeBlock* cb, string result_reg){}

private:
    int val;
    string type;
};

class ConstDouble_Node : public Const_value_Node{
public:
    ConstDouble_Node(double val): val(val){type = ".double";}
    string get_val() { return to_string(val); }
    string get_type() {return type;}
    void gen_data(CodeGenerator* cg);
    void gen_compute_code(CodeBlock* cb, string result_reg){}
private:
    double val;
    string type;
};

class ConstChar_Node : public Const_value_Node{
public:
    ConstChar_Node(char val): val(val){type = ".byte";}
    string get_val() { return "'" + to_string(val) + "'"; }
    string get_type() {return type;}
    void gen_data(CodeGenerator* cg);
    void gen_compute_code(CodeBlock* cb, string result_reg){}
private:
    char val;
    string type;
};

class ConstBool_Node : public Const_value_Node{
public:
    ConstBool_Node(bool val): val(val){type = ".byte";}
    string get_val() { if (val) return "1"; else return "0"; }
    string get_type() {return type;}
    void gen_data(CodeGenerator* cg);
    void gen_compute_code(CodeBlock* cb, string result_reg){}
private:
    bool val;
    string type;
};

class ConstStr_Node : public Const_value_Node{
public:
    ConstStr_Node(const string& val): val(val){type = ".asciiz";}
    string get_val() { return "\"" + val + "\""; }
    string get_type() {return type;}
    void gen_data(CodeGenerator* cg);
    void gen_compute_code(CodeBlock* cb, string result_reg){}
private:
    string val;
    string type;
};


class Factor_id_Node : public Factor_Node{
public:
    Factor_id_Node(Id_Node* id):id(id){}
    string build_symbol_table(string type);
    void gen_compute_code(CodeBlock* cb, string result_reg);

private:
    Id_Node *id;
};

class Factor_unary_Node : public Factor_Node{
public:
    enum Type {NOT, MINUS};
public:
    Factor_unary_Node(Type type, Factor_Node *factor): type(type), factor(factor){}
    string build_symbol_table(string type);
    void gen_code(CodeGenerator* cg, int block_id) {}

private:
    Type type;
    Factor_Node *factor;
};

class Func_call_Node : public Factor_Node{
    //function call (not procedure)
public:
    Func_call_Node(Id_Node* id, Args_list_Node* args): id(id), args(args){}
    explicit Func_call_Node(Id_Node* id): id(id), args(nullptr){}
    string build_symbol_table(string type);
    void gen_code(CodeGenerator* cg, int block_id) {}

private:
    Id_Node* id;
    Args_list_Node* args;
};

class Factor_arr_Node : public Factor_Node{
public:
    Factor_arr_Node(Id_Node *id, Expression_Node *index)
        :id(id), index(index){}
    string build_symbol_table(string);
    void gen_code(CodeGenerator* cg, int block_id) {}

private:
    Id_Node *id;
    Expression_Node *index;
};

class Factor_record_Node : public Factor_Node{
public:
    Factor_record_Node(Id_Node *record, Id_Node *member)
        :record(record), member(member){}
    string build_symbol_table(string type);
    void gen_code(CodeGenerator* cg, int block_id) {}

private:
    Id_Node *record;
    Id_Node *member;
};

class Expr_Node : public TreeNode{
    //Expr and Term
public:
    enum Op_type {AND, OR, ADD, SUB, MUL, DIV, MOD, NONE};
public:
    //exp op exp
    Expr_Node(Expr_Node *expr_lhs, Op_type type, Expr_Node *expr_rhs)
        :type(type), expr_lhs(expr_lhs), expr_rhs(expr_rhs){}

    //factor
    explicit Expr_Node(Factor_Node *factor)
        :type(NONE), factor(factor){}
    string build_symbol_table(string type);
    void gen_compute_code(CodeBlock* cb, string result_reg);

private:
    Op_type type;
    union{
        struct{
            Expr_Node* expr_lhs;
            Expr_Node* expr_rhs;    
        };
        Factor_Node* factor;
    };
};

class Expression_Node : public Factor_Node{
public:
    enum Cmp_type{GE, GT, LE, LT, EQ, NEQ, NONE};
public:
    Expression_Node(Expression_Node* expression, Cmp_type type,
        Expr_Node* expr):type(type), expression(expression), expr(expr){}
    Expression_Node(Expr_Node* expr)
        :type(NONE), expression(nullptr), expr(expr){}
    void gen_compute_code(CodeBlock* cb, string result_reg);

    string build_symbol_table(string type);

private:
    Cmp_type type;
    Expression_Node* expression;//may be null, first evaluate
    Expr_Node* expr;
};

class Expression_list_Node : public TreeNode{
public:
    Expression_list_Node(Expression_list_Node* prev, Expression_Node* node)
        :prev(prev), node(node){}
    explicit Expression_list_Node(Expression_Node* node):prev(nullptr), node(node){}
    void gen_code(CodeGenerator* cg, int block_id) {}
private:
    Expression_list_Node* prev;
    Expression_Node* node;
};

#endif
