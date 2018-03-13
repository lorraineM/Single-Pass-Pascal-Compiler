#ifndef _TREENODE_
#define _TREENODE_

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/
# include <string>
# include <vector>
# include "../symtab/symboltable.h"
# include "../codegen/codegen.hpp"

using namespace std;

/* Pre-declarations. */
class TreeNode; // General abstract class.

class Id_Node; // from ExpNode
class Type_decl_Node;  // from DeclNode
class Const_value_Node;
class CodeGenerator; // from codegen

class Type_definition_Node; // Id_node, Type_decl_Node
class Var_decl_Node; // Name_list_Node, Type_decl_Node
class Const_expr_list_Node; // Const_expr_list_Node, Id_Node, Const_value_Node
class Type_decl_list_Node; // Type_decl_list_Node, Type_definition_Node
class Name_list_Node; // Name_list_Node, Id_Node
class Var_decl_list_Node; // Var_decl_list_Node, Var_decl_Node

class TreeNode{
public:
    virtual ~TreeNode(){}
    virtual int gen_code(CodeGenerator* cg) {}
    virtual void gen_code(CodeGenerator* cg, int block_id) {}
    // virtual string build_symbol_table(string type = "");
    
public:
    int getLineno() const{
        return lineno;
    }

    void setLineno(int lineno){
        this->lineno = lineno;
    }
private:
    int lineno;
};
/********************* node **********************/
class Type_definition_Node : public TreeNode
{
public:
    Type_definition_Node(Id_Node* id, Type_decl_Node* type_decl)
        :id(id), type_decl(type_decl){}
    string build_symbol_table(string type = "");

private:
    Id_Node* id;
    Type_decl_Node* type_decl;
    /* data */
};

class Var_decl_Node : public TreeNode
{
public:
    Var_decl_Node(Name_list_Node* name_list, Type_decl_Node *type)
        :name_list(name_list), type(type){}
    void parse_var(CodeGenerator* cg, int block_id);
    string build_symbol_table(string type = "");

private:
    Name_list_Node * name_list;
    Type_decl_Node * type;
};

/********************* list *********************/
class Const_expr_list_Node : public TreeNode{
public:
    Const_expr_list_Node(Const_expr_list_Node *prev, 
        Id_Node* id, Const_value_Node *const_value)
        :prev(prev), id(id), const_value(const_value){}
    explicit Const_expr_list_Node(Id_Node* id, Const_value_Node *const_value)
        :prev(nullptr), id(id), const_value(const_value){}
    void gen_data(CodeGenerator* cg);
    string build_symbol_table(string type = "");
    Const_expr_list_Node* get_prev(){return this->prev;}

private:
    Const_expr_list_Node *prev;
    Id_Node* id;
    Const_value_Node *const_value;
};

class Type_decl_list_Node : public TreeNode{
public:
    Type_decl_list_Node(Type_decl_list_Node* prev, Type_definition_Node* type_def)
        :prev(prev), type_def(type_def){}
    explicit Type_decl_list_Node(Type_definition_Node* type_def)
        :prev(nullptr), type_def(type_def){}
    string build_symbol_table(string type = "");
    Type_decl_list_Node* get_prev(){return this->prev;}

private:
    Type_decl_list_Node* prev;
    Type_definition_Node* type_def;
};

class Name_list_Node : public TreeNode
{
public:
    Name_list_Node(Name_list_Node *prev, Id_Node* id)
        :prev(prev), id(id){}
    explicit Name_list_Node(Id_Node* id)
        :prev(nullptr), id(id){}
    void parse_var(CodeGenerator* cg, int block_id);
    void parse_para(CodeGenerator* cg, int block_id);
    string build_symbol_table(string type = "");
    Name_list_Node* get_prev(){return this->prev;}
    Id_Node* get_id(){return this->id;}

private:
    Name_list_Node *prev;
    Id_Node* id;
};

class Var_decl_list_Node : public TreeNode
{
public:
    Var_decl_list_Node(Var_decl_list_Node* prev, Var_decl_Node* decl)
        :prev(prev), var_decl(decl){}
    explicit Var_decl_list_Node(Var_decl_Node* decl)
        :prev(nullptr), var_decl(decl){}
    void parse_var(CodeGenerator* cg, int block_id);
    string build_symbol_table(string type = "");
    Var_decl_list_Node* get_prev(){return this->prev;}

private:
    Var_decl_list_Node * prev;
    Var_decl_Node * var_decl;
};



#endif
