#include "symboltable.h"

#include "../node/TreeNode.hpp"
#include "../node/DeclNode.hpp"
#include "../node/ExpNode.hpp"
#include "../node/ModuleNode.hpp"
#include "../node/StmtNode.hpp"

using namespace std;

/*ModuleNode*/
//string build_symbol_table(string type)
//return type

//1. program head
string Program_Node::build_symbol_table(string type) {
	if (this->head != nullptr) {
		this->head->build_symbol_table("");
	}

	if (this->routine != nullptr) {
		this->routine->build_symbol_table("");
	}
	return "";
}

string Program_head_Node::build_symbol_table(string type) {
	if (this->id != nullptr) {
		// table_unit *p = st->st_insert(this->id->get_name(), this->id->getLineno(), 0, "program_id");
		//insert program head for fun
	}
	return "";
}


//1. program routine
string Routine_Node::build_symbol_table(string type) {
	if (this->head != nullptr) {
		this->head->build_symbol_table("");
	}
	if (this->body != nullptr) {
		this->body->build_symbol_table("");	
	}
	return "";
}

// 2. program routine_head
string Routine_head_Node::build_symbol_table(string type) {
	if (this->const_part != nullptr) {
		this->const_part->build_symbol_table("");
	}
	if (this->type_part != nullptr) {
		this->type_part->build_symbol_table("");	
	}
	if (this->var_part != nullptr) {
		this->var_part->build_symbol_table("");
	}
	if (this->routine_part != nullptr) {
		this->routine_part->build_symbol_table("");
	}
	return "";
}


// 3. routine_head_node
string Const_part_Node::build_symbol_table(string type) {
	if (this->const_expr_list != nullptr) {
		this->const_expr_list->build_symbol_table("");
	}
	return "";
}


string Const_expr_list_Node::build_symbol_table(string type) {
	st->st_insert(this->id->get_name(), this->id->getLineno(), 0, "const");

	if (this->prev != nullptr) {
		this->prev->build_symbol_table("");
	}
	return "";
}

string Type_part_Node::build_symbol_table(string type) {	
	if (this->list != nullptr) {
		this->list->build_symbol_table("");
	}
	return "";
}


string Type_decl_list_Node::build_symbol_table(string type) {	//Treenode
	if (this->prev != nullptr) {
		this->prev->build_symbol_table("");
	}

	if (this->type_def != nullptr) {
		this->type_def->build_symbol_table("");	//todo
	}
	return "";
}

string Type_definition_Node::build_symbol_table(string type) { 	//Treenode
	string type_name;
	if (this->id != nullptr) {
		table_unit *p = st->st_insert(this->id->get_name(), this->id->getLineno(), 0, type_name); 
		this->id->sym_unit = p;
		type_name = this->id->get_name();
	}

	if (this->type_decl != nullptr) {
		this->type_decl->build_symbol_table(type_name);
	}

	return "";
}


string Var_part_Node::build_symbol_table(string type) {
	if (this->list != nullptr) {
		this->list->build_symbol_table("");
	}
	return "";
}


string Var_decl_list_Node::build_symbol_table(string type) {
	if (this->var_decl != nullptr) {
		this->var_decl->build_symbol_table("");
	}

	if (this->prev != nullptr) {
		this->prev->build_symbol_table("");
	}
	return "";
}


string Var_decl_Node::build_symbol_table(string type) {	//OK
	string v_name;
	if (this->name_list != nullptr) {
		this->name_list->build_symbol_table("");
	}//insert_names
	Name_list_Node *p= this->name_list;
	while (p->get_prev() != nullptr) {
		if (this->type != nullptr) {
			this->type->build_symbol_table(p->get_id()->get_name());
		}
		printf("000000%p\n", p->get_id()->sym_unit);
		p = p->get_prev();
	}

	if (this->type != nullptr) {
		this->type->build_symbol_table(p->get_id()->get_name());
	}
	return "";
}




string Name_list_Node::build_symbol_table(string type) {	//OK
	if (this->prev != nullptr) {
		this->prev->build_symbol_table(type);
	}

	if (this->id != nullptr) {
		table_unit *p = st->st_insert(this->id->get_name(), this->id->getLineno(), 0, "");
		this->id->sym_unit = p; //reverse-link
	}
	return "";
}





string Routine_part_Node::build_symbol_table(string type) {
	if (this->prev != nullptr) {
		this->prev->build_symbol_table("");
	}
	if (this->get_isfunction()) {
		this->func->build_symbol_table("");
	} else {
		this->proc->build_symbol_table("");
	}
	return "";
}


string Function_decl_Node::build_symbol_table(string type) {
	if (this->id != nullptr) {
		st->st_insert(this->id->get_name(), this->id->getLineno(), 0, "function");
		//create new table
		symboltable *new_st = new symboltable();
		printf("%p", new_st);
		//puts("");
		new_st->forward = st;
		st->st_func_proc(this->id->get_name(), new_st);
		st = new_st;
	}

	if (this->paras != nullptr) {
		this->paras->build_symbol_table("");
	}

	if (this->ret_type != nullptr) {
		this->ret_type->build_symbol_table("");		
	}
	if (this->routine != nullptr) {
		this->routine->build_symbol_table("");
	}

	//point back to old table
	if (this->id != nullptr) {
		st = st->forward;
	}
	return "";
}


string Parameters_Node::build_symbol_table(string type) {
	if (this->list != nullptr) {
		this->list->build_symbol_table("");
	}
	return "";
}

string Para_decl_list_Node::build_symbol_table(string type) {
	if (this->prev != nullptr) {
		this->prev->build_symbol_table("");
	}
	if (this->type != nullptr) {
		this->type->build_symbol_table("");
	}
	return "";
}


string Para_type_list_Node::build_symbol_table(string type) {
	string type_name = " ";
	if (this->isVal) {
		this->val_para_list->get_list()->build_symbol_table();
		Name_list_Node *p = this->val_para_list->get_list();

		while (p->get_prev() != nullptr) {
			this->val_para_list->build_symbol_table(p->get_id()->get_name());
			table_unit *t = st->st_lookup(p->get_id()->get_name());
			t->isref = 1;
			p = p->get_prev();
		}
		this->val_para_list->build_symbol_table(p->get_id()->get_name());
	} else {
		this->var_para_list->get_list()->build_symbol_table();
		Name_list_Node *p = this->var_para_list->get_list();

		while (p->get_prev() != nullptr) {
			this->var_para_list->build_symbol_table(p->get_id()->get_name());
			table_unit *t = st->st_lookup(p->get_id()->get_name());
			t->isref = 0;
			p = p->get_prev();
		}
		this->var_para_list->build_symbol_table(p->get_id()->get_name());
	}
	return "";
}

string Var_para_list_Node::build_symbol_table(string type) {
	if (this->list != nullptr) {
		this->list->build_symbol_table("");
	}
	return "";
}


string Val_para_list_Node::build_symbol_table(string type) {
	if (this->list != nullptr) {
		this->list->build_symbol_table("");
	}
	return "";
}


string Procedure_decl_Node::build_symbol_table(string type) {
	if (this->id != nullptr) {
		st->st_insert(this->id->get_name(), this->id->getLineno(), 0, "procedure");
		//create new table
		symboltable *new_st = new symboltable();
		new_st->forward = st;
		st->st_func_proc(this->id->get_name(), new_st);
		st = new_st;

	}

	if (this->paras != nullptr) {
		this->paras->build_symbol_table("");
	}

	if (this->routine != nullptr) {
		this->routine->build_symbol_table("");
	}
	//back
	if (this->id != nullptr) {
		st = st->forward;
	}
	return "";
}


string Routine_body_Node::build_symbol_table(string type) {
	if (this->stmts != nullptr) {
		this->stmts->build_symbol_table();
	}
	return "";
}



string Stmt_Node::build_symbol_table(string type) {
	//puts("---stmt_in---");
	if (this->get_hasLable()) {
		//puts("how to deal with label");
		this->stmt->build_symbol_table(type);
	} else {
		this->stmt->build_symbol_table(type);
	}						
	return "";
}


//================stmt node=======================
string Assign_stmt_Node::build_symbol_table(string type) {
	//puts("===assign===");
	if (this->expr != nullptr) {
		this->expr->build_symbol_table("");
	}
	return "";
}


string Assign_id_stmt_Node::build_symbol_table(string type) {
	//puts("===assign_id===");
	string type_name;
	if (this->expr != nullptr) {
		this->expr->build_symbol_table("");
	}
	if (this->id != nullptr) {
		//check
		table_unit *p = st->st_lookup(this->id->get_name());
		this->id->sym_unit = p;
	}
	return "";
}

string Assign_arr_stmt_Node::build_symbol_table(string type) {
	//puts("===assign_arr===");
	string type_name;	//arr_name, index, expr,
	if (this->arr_name != nullptr) {
		//check
		table_unit *p = st->st_lookup(this->arr_name->get_name());
		this->arr_name->sym_unit = p;
	}
	if (this->index != nullptr) {
		this->index->build_symbol_table("");
	}
	if (this->expr != nullptr) {
		this->expr->build_symbol_table("");
	}
	return "";
}


string Assign_record_stmt_Node::build_symbol_table(string type) {
	//puts("===assign_rec===");
	if (this->record_name != nullptr) {
		table_unit *p = st->st_lookup(this->record_name->get_name());
		this->record_name->sym_unit = p;
	}
	if (this->member != nullptr) {
		table_unit *p = st->st_lookup(this->member->get_name());
		this->member->sym_unit = p;
		//check
	}
	if (this->expr != nullptr) {
		this->expr->build_symbol_table("");
	}
	return "";
}


string Proc_stmt_Node::build_symbol_table(string type) {
	//puts("===proc_stmt_node===");
	if (this->id != nullptr) {
		table_unit *p = st->st_lookup(this->id->get_name());
		this->id->sym_unit = p;
	}
	if (this->args != nullptr) {
		this->args->build_symbol_table("");
	}
	return "";
}


string Read_stmt_Node::build_symbol_table(string type) {
	//puts("===read===");
	if (this->id != nullptr) {
		table_unit *p = st->st_lookup(this->id->get_name());
		this->id->sym_unit = p;									//re-link of id in read
	}
	return "";
}

string Write_stmt_Node::build_symbol_table(string type) {
	//puts("====write====");
	if (this->expression != nullptr) {
		this->expression->build_symbol_table("");
	}
	return "";
}
string
 Writeln_stmt_Node::build_symbol_table(string type) {
	//puts("writeln");
	if (this->expression != nullptr) {
		this->expression->build_symbol_table("");
	}
	return "";
}

string Compound_stmt_Node::build_symbol_table(string type) {
	//puts("===Compound===");
	if (this->stmts != nullptr) {
		//puts("---Compound_stmt_Node---");
		this->stmts->build_symbol_table("");
	}
	return "";
}

string Else_clause_Node::build_symbol_table(string type) {
	//puts("===else===");
	if (this->stmt != nullptr) {
		this->stmt->build_symbol_table("");
	}
	return "";
}

string If_stmt_Node::build_symbol_table(string type) {
	//puts("===if_stmt===");
	if (this->exp != nullptr) {
		this->exp->build_symbol_table("");
	}
	if (this->stmt != nullptr) {
		this->stmt->build_symbol_table("");
	}
	if (this->else_clause != nullptr) {
		this->stmt->build_symbol_table("");
	}
	return "";
}

string Repeat_stmt_Node::build_symbol_table(string type) {
	//puts("===repeat===");
	if (this->stmts != nullptr) {
		this->stmts->build_symbol_table("");
	}
	if (this->expr != nullptr) {
		this->expr->build_symbol_table("");
	}
	return "";
}

string While_stmt_Node::build_symbol_table(string type) {
	//puts("===while===");
	if (this->expr != nullptr) {
		this->expr->build_symbol_table("");
	}
	if (this->stmt != nullptr) {
		this->stmt->build_symbol_table("");
	}
	return "";
}

string For_stmt_Node::build_symbol_table(string type) {
	//puts("===for===");
	if (this->id != nullptr) {
		table_unit *p = st->st_lookup(this->id->get_name());
		this->id->sym_unit = p;
		//check
	}
	if (this->id_exp != nullptr) {
		this->id_exp->build_symbol_table("");
	}
	if (this->exp != nullptr) {
		this->exp->build_symbol_table("");
	}
	if (this->stmt != nullptr) {
		this->stmt->build_symbol_table("");
	}
	return "";
}


string Case_expr_Node::build_symbol_table(string type) {
	//puts("===case_expr===");
	if (this->stmt != nullptr) {
		this->stmt->build_symbol_table();
	}
	return "";
}

string Case_const_val_expr_Node::build_symbol_table(string type) {
	//puts("===case const===");
	//ignore const_val, for no id
	if (this->stmt != nullptr) {
		this->stmt->build_symbol_table("");
	}
	return "";
}

string Case_id_expr_Node::build_symbol_table(string type) {
	//puts("===case id===");
	if (this->id != nullptr) {
		table_unit *p = st->st_lookup(this->id->get_name());
		this->id->sym_unit = p;
		//check
	}

	if (this->stmt != nullptr) {
		this->stmt->build_symbol_table("");
	}
	return "";
}

string Case_expr_list_Node::build_symbol_table(string type) {
	//puts("===case expr===");
	if (this->prev != nullptr) {
		this->prev->build_symbol_table("");
	}
	if (this->node != nullptr) {
		this->node->build_symbol_table("");
	}
	return "";
}


string Case_stmt_Node::build_symbol_table(string type) {
	//puts("===case stmt===");
	if (this->expr != nullptr) {
		this->expr->build_symbol_table("");
	}
	if (this->list != nullptr) {
		this->list->build_symbol_table("");
	}
	return "";
}

string Goto_stmt_Node::build_symbol_table(string type) {
	return "";
}

string Stmt_list_Node::build_symbol_table(string type) {
	//puts("===stmt list===");
	if (this->prev != nullptr) {
		this->prev->build_symbol_table("");
	}

	if (this->stmt != nullptr) {
		this->stmt->build_symbol_table("");
	}
	return "";
}

// //============ end of === stmt node===============


// //==================Exp Node======================
string Const_value_Node::build_symbol_table(string type) {
	return "";
}
string Expression_Node::build_symbol_table(string type) {
	//Cmp_type is compare_operator, ignore;
	//puts("***expression_node***");
	if (this->expression != nullptr) {
		this->expression->build_symbol_table("");
	}
	if (this->expr != nullptr) {
		this->expr->build_symbol_table("");
	}
	return "";
}

string Expr_Node::build_symbol_table(string type) {
	// ignore op_type
	//puts("***expr_node***");
	// printf("%p, %p, %p", this->expr_lhs, this->expr_rhs, this->factor);
	// printf("%d", this->getLineno());
	// //puts("");
	
	if (this->type != NONE) {
		if (this->expr_lhs != nullptr) {
			this->expr_lhs->build_symbol_table("");
		}
		if (this->expr_rhs != nullptr){	//union
			this->expr_rhs->build_symbol_table("");
		}
	} else {
		if (this->factor != nullptr) {
			this->factor->build_symbol_table("");
		}
	}
	return "";
}

string Factor_id_Node::build_symbol_table(string type) {
	//puts("Factor_id_Node");
	if (this->id != nullptr) {
		table_unit *p = st->st_lookup(this->id->get_name());
		this->id->sym_unit = p;
		//check
	}
	return "";
}

string Factor_unary_Node::build_symbol_table(string type) {
	//puts("Factor_unary_Node");
	//ignore type, for {NOT, MINUS}
	if (this->factor != nullptr) {
		this->factor->build_symbol_table("");
	}
	return "";
}

string Func_call_Node::build_symbol_table(string type) {
	//puts("Func_call_Node");
	if (this->id != nullptr) {
		//st_check
		table_unit *p = st->st_lookup(this->id->get_name());
		this->id->sym_unit = p;
	}
	if (this->args != nullptr) {
		//if id_check-->so do it;
		this->args->build_symbol_table("");
	}
	return "";
}

string Factor_arr_Node::build_symbol_table(string type) {
	//puts("Factor_arr_Node");
	if (this->id != nullptr) {
		//st_check
		table_unit *p = st->st_lookup(this->id->get_name());
		this->id->sym_unit = p;
	}
	if (this->index != nullptr) {
		this->index->build_symbol_table("");
	}
	return "";
}

string Factor_record_Node::build_symbol_table(string type) {
	//puts("Factor_record_Node");
	if (this->record != nullptr) {
		//st_check	whether member in 
		//make_index
		table_unit *p = st->st_lookup(this->record->get_name());
		this->record->sym_unit = p;
	}
	if (this->member != nullptr) {
		//st_check
		//make_index
		table_unit *p = st->st_lookup(this->member->get_name());
		this->member->sym_unit = p;
	}
	return "";
}


// //===============end of exp node===================



//  ======================  args  ====================
string Args_list_Node::build_symbol_table(string type) {
	//puts("Args_list_Node");
	if (this->prev != nullptr) {
		this->prev->build_symbol_table("");
	}
	if (this->expr != nullptr) {
		this->expr->build_symbol_table("");
	}
	return "";
}
//=======================end of args =================




//==============type declaim ====================
string Array_type_decl_Node::build_symbol_table(string type) {
	string type_name;
	string type_range;
	puts("array");
	if (this->type != nullptr) {
		type_name = this->type->build_symbol_table(type);
	}
	if (this->range != nullptr) {
		type_range = this->range->build_symbol_table(type);
	}
	return type_name;
}

string Field_decl_Node::build_symbol_table(string type) {
	string type_name;
	puts("field");
	if (this->name_list != nullptr) {
		this->name_list->build_symbol_table(type_name);
	}

	if (this->type != nullptr) {
		this->type->build_symbol_table(type);		//record type_name
	}
	return "";
}


string Field_decl_list_Node::build_symbol_table(string type) {
	puts("field_dec");
	if (this->prev != nullptr) {
		this->prev->build_symbol_table(type);
	}
	if (this->decl != nullptr) {
		this->decl->build_symbol_table(type);
	}
	return "";
}


string Record_type_decl_Node::build_symbol_table(string type) {
	puts("record");
	if (this->list != nullptr) {
		this->list->build_symbol_table(type);
	}
	return "";
}

string System_type_decl_Node::build_symbol_table(string type) {
	Type sys_type = this->getType();
	table_unit * t_unit = st->st_lookup(type);
	if (t_unit != nullptr) {
		if (sys_type == INT) 	t_unit->type = ".word";
		if (sys_type == REAL) 	t_unit->type = ".double";
		if (sys_type == ENUM) 	t_unit->type = ".enum";
		if (sys_type == CHAR) 	t_unit->type = ".byte";
		if (sys_type == STRING) t_unit->type = ".asciiz";
		if (sys_type == BOOL) 	t_unit->type = ".byte";
	} else {
	}
	return "";
}

string Alias_type_decl_Node::build_symbol_table(string type) {
	string type_name;
	if (this->id != nullptr) {
		type_name = this->id->get_name();
		table_unit *t = st->st_lookup(type);
		t->type = type_name;
	}
	return "";
}

string Enum_type_decl_Node::build_symbol_table(string type) {
	if (this->name_list != nullptr) {
		this->name_list->build_symbol_table(type);
	}
	return "";
}



string Subrange_const_value_type_decl_Node::build_symbol_table(string type) {
	int low = this->lowerBound->get_value();
	if (this->isLowerNeg) low = -low;
	int up = this->upperBound->get_value();
	if (this->isUpperNeg) up = - up;
	table_unit *t = st->st_lookup(type);
	t->array_start = low;
	t->volumn = up - low + 1;
	t->isarray = 1;
	return "";
}

string Subrange_id_type_decl_Node::build_symbol_table(string type) {
	// I've work over 7 hours, but did not finish... T_T//
	return "";
}
//================end of declaim ================
