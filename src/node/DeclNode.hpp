#ifndef _DECLNODE_
#define _DECLNODE_

#include "TreeNode.hpp"
#include "../symtab/symboltable.h"
#include "../codegen/codegen.hpp"

using namespace std;

class Type_part_Node; // Type_decl_list_Node;
class Type_decl_Node;
class Array_type_decl_Node; // Simple_type_decl_Node, Type_decl_Node

class Field_decl_Node; // Name_list_Node, Type_decl_Node
class Field_decl_list_Node; // Field_decl_list_Node, Field_decl_Node
class Record_type_decl_Node; // Field_decl_list_Node

class Simple_type_decl_Node;
class System_type_decl_Node;
class Alias_type_decl_Node; // Id_Node
class Enum_type_decl_Node; // Name_list_Node
class Subrange_type_decl_Node;
class Subrange_const_value_type_decl_Node;
class Subrange_id_type_decl_Node;

class Type_part_Node : public TreeNode{
public:
	explicit Type_part_Node(Type_decl_list_Node *list)
		:list(list){}
	string build_symbol_table(string type = "");
	
private:
	Type_decl_list_Node * list;
};

class Type_decl_Node : public TreeNode{
public:
	virtual string build_symbol_table(string){};

protected:
	Type_decl_Node(){}
};


class Simple_type_decl_Node;
class Array_type_decl_Node : public Type_decl_Node{
public:
	Array_type_decl_Node(Simple_type_decl_Node *range, Type_decl_Node *type)
		:range(range), type(type){}
  void gen_code(CodeGenerator* cg, int block_id) {}
	string build_symbol_table(string);

public:
	Simple_type_decl_Node * range;
	Type_decl_Node * type;
};

class Field_decl_Node : public TreeNode{ //for record type
public:
	Field_decl_Node(Name_list_Node *name_list, Type_decl_Node *type)
		:name_list(name_list), type(type){}
  void gen_code(CodeGenerator* cg, int block_id) {}
	string build_symbol_table(string);

private:
	Name_list_Node *name_list;
	Type_decl_Node *type;
};

class Field_decl_list_Node : public TreeNode{
public:
	Field_decl_list_Node(Field_decl_list_Node *prev, Field_decl_Node *decl)
		:prev(prev), decl(decl){}

	explicit Field_decl_list_Node(Field_decl_Node *decl)
		:decl(decl){}
  void gen_code(CodeGenerator* cg, int block_id) {}
	string build_symbol_table(string);

private:
	Field_decl_list_Node *prev = nullptr;
	Field_decl_Node *decl;
};

class Record_type_decl_Node : public Type_decl_Node{
public:
	explicit Record_type_decl_Node(Field_decl_list_Node* list)
        :list(list){}
  void gen_code(CodeGenerator* cg, int block_id) {}
  string build_symbol_table(string);

private:
	Field_decl_list_Node *list;
};

/*********  simple type decl *****************/
class Simple_type_decl_Node : public Type_decl_Node{
protected:
	Simple_type_decl_Node(){}
};

class System_type_decl_Node : public Simple_type_decl_Node{
public:
	enum Type{INT, REAL, ENUM, CHAR, STRING, BOOL};
public:
	explicit System_type_decl_Node(Type type):type(type){}
	Type getType() const{ 
        return type;
    }
  void gen_code(CodeGenerator* cg, int block_id) {}
  string build_symbol_table(string);

private:
	Type type;
};

class Alias_type_decl_Node : public Simple_type_decl_Node{
public:
	explicit Alias_type_decl_Node(Id_Node* id):id(id){}
  void gen_code(CodeGenerator* cg, int block_id) {}
	string build_symbol_table(string);
private:
	Id_Node *id;
};

class Enum_type_decl_Node : public Simple_type_decl_Node{
public:
	explicit Enum_type_decl_Node(Name_list_Node *name_list)
		:name_list(name_list){}
  void gen_code(CodeGenerator* cg, int block_id){}
	string build_symbol_table(string);

private:
	Name_list_Node *name_list;
};

class Subrange_type_decl_Node : public Simple_type_decl_Node{
public:
  void gen_code(CodeGenerator* cg, int block_id){}
protected:
	Subrange_type_decl_Node(){}
};

class Subrange_const_value_type_decl_Node : public Subrange_type_decl_Node
{
public:
	Subrange_const_value_type_decl_Node(bool lowerNeg, Const_value_Node *low, 
		bool upperNeg, Const_value_Node *high)
		:lowerBound(low), upperBound(high), isLowerNeg(lowerNeg), isUpperNeg(upperNeg){}
  void gen_code(CodeGenerator* cg, int block_id){}
	string build_symbol_table(string);

private:
	Const_value_Node *lowerBound;
	Const_value_Node *upperBound;
	bool isLowerNeg;	//is lower bound neggative
	bool isUpperNeg;	//is upper bound neggative
};

class Subrange_id_type_decl_Node : public Subrange_type_decl_Node
{
public:
	Subrange_id_type_decl_Node(Id_Node* lower, Id_Node* upper)
		:lower(lower), upper(upper){}
  void gen_code(CodeGenerator* cg, int block_id){}
	string build_symbol_table(string);
	
private:
	Id_Node* lower;
	Id_Node* upper;
};

#endif
