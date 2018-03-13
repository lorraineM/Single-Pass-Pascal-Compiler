#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include <iostream>
#include <string>


#define SIZE 211
#define SHIFT 4

class links_recorder;
class table_unit;
class symboltable;


class lines_recorder
{
public:
	lines_recorder() {};
	~lines_recorder() {};

	/* data */
public:
	int lineno;
	lines_recorder * next;
};


/** table_unit store a single bins**/
class table_unit
{
public:
	table_unit() {this->use = 0;};
	~table_unit() {};

	/* data */
public:
	//local use
	table_unit * next;
	lines_recorder *lines;	
	int use;	//record usage, if is in use
	symboltable * func_proc_link;	//if function, link to another table;


	//name
	std::string name;		//name of variable or const;
	//type
	std::string type;		//"const",  variable : "integer",  "float",  "function", "proc" ...
	//if array
	int isarray;			//array or not
	int array_start;		//array_start
	int volumn;				//[100] (if have)
	//if record
	int isrecord;			//if record
	std::string belong;		//belong to which type
	//if func_proc
	int isref;				//if func_proc

	//designed for code-gen
	int memloc;				//record memory_location
};



/** symboltable, store array of bins**/
class symboltable
{
public:
	symboltable(){ this->forward = this; this->global = 0;};
	~symboltable(){};
	

	table_unit * st_lookup(std::string name) {
		int h = this->hash_find_unit(name);
		symboltable *p = this;
		table_unit *l = &(p->units[h]);

		while ((l->use == 1) && (name.compare(l->name) != 0)) {
			l = l->next;
		}
		if ((l->use == 1) && (name.compare(l->name) == 0)) {
			return l;
		}
		//--------------
		while (p->forward != p) {//find along forward
			table_unit *l = &(p->units[h]);

			while ((l->use == 1) && (name.compare(l->name) != 0)) {
				l = l->next;
			}
			if ((l->use == 1) && (name.compare(l->name) == 0)) {
				return l;
			}
			p = p->forward;
		}
		return nullptr;
	}

	/**
	  *	assign st to the func's symbol table
	  *	@param	name: of func, st: symbol table
	  */
	void st_func_proc(std::string name, symboltable *st) {
		int h = this->hash_find_unit(name);
		// table_unit * l = new table_unit;
		// l = &this->units[h];
		table_unit * l = &this->units[h];
		while ((l->use != 0) && (name.compare(l->name) != 0)) {
			l = l->next;
		}

		//if find func, assign st to its link
		if ((l->use == 1) && (name.compare(l->name) == 0)) {
			l->func_proc_link = st;
		}
	}

	/**
	  *	add table_unit to the table		
	  *
	  */
	table_unit * st_insert(std::string name, int lineno, int loc, std::string type) {
		int h = this->hash_find_unit(name);

		table_unit *l = &(this->units[h]);
		l->name = name;

		while ((l->use != 0) && (name.compare(l->name) != 0)) {
			l = l->next;
// <<<<<<< HEAD
// 		}
// 		printf("%d\n", l->use);
// 		if (l->use == 1) {
// =======
		}//table_unit is a list
		//TODO: l == nullptr ?

		if (l->use == 1) {//empty
// >>>>>>> c2f6171731767e0de9b639c924a2545e5a2a5b9d
			l = new table_unit();
			l->use = 1;
			l->name = name;
			l->type = type;
			l->lines = new lines_recorder();
			l->lines->lineno = lineno;
			l->lines->next = nullptr;
			l->next = &units[h];
// <<<<<<< HEAD
			units[h] = *l;
		} else {
			l->name = name;
			l->type = type;
			l->use = 1;
		}
		// printf("test: %p\n", l);
		return l;
// =======
// 			units[h] = *l; //insert to the front
// 		} else {//already exists
// 			lines_recorder *t = l->lines;
// 			while (t->next != nullptr)
// 				t = t->next;
// 			t->next = new lines_recorder;
// 			t->next->lineno = lineno;
// 			t->next->next = nullptr;
// 			//add lines_recorder to the list
// 		}

// 		printf("finish insert: --------- %s", name.c_str());
// 		puts("");
// 		printf("type: %s", type.c_str());
// 		puts("");
// 		return l; //TODO return a copy's address?
// >>>>>>> c2f6171731767e0de9b639c924a2545e5a2a5b9d
	}

public:
	//	get an hash value according to the name
	int hash_find_unit(std::string name){ 
		int res = 0;
		for (int i=0; i<name.length(); i++) {
			res = ((res << SHIFT) + name[i]) % SIZE;
		}
		return res;
	};

	symboltable * forward;//list of symbol table
	//init as this
public:
	table_unit units[SIZE];//all init as use = 0
	int global;
};


extern symboltable* st;

#endif