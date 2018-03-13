#ifndef _CODE_GEN_H
#define _CODE_GEN_H

#include "../node/TreeNode.hpp"
#include <vector>
#include <cstdio>
#include <map>

using namespace std;

typedef map<string, string> VariableMap;

class TreeNode;

class CodeBlock {
public:
  VariableMap variable_map;
  int var_count;
  int para_count;
  int temp_var_count;
  CodeBlock(string name) {
    this->name = name;
    this->code = name + ":\n";
    this->var_count = 0;
    this->para_count = 0;
  }
  ~CodeBlock() {}
  void add_line(string line) {code += "  " + line + "\n";}
  string generated_code() { return code; }
  string get_name() { return name; }
private:
  string name;
  string code;
};

class DataBlock {
public:
  DataBlock(string data) {this->data = "  " + data + "\n";}
  ~DataBlock() {}
  string generated_data() {return data;}
private:
  string data;
};

class CodeGenerator {
public:
  vector<CodeBlock*> code_blocks;
  vector<DataBlock*> data_blocks;

  CodeGenerator(TreeNode* syntax_tree, char* code_file_name)
  {
    this->syntax_tree = syntax_tree;

    this->code_file = fopen(code_file_name, "w");
  }
  ~CodeGenerator()
  {
    fclose(this->code_file);
  }
  void generate();
private:
  TreeNode *syntax_tree;
  FILE* code_file;

  void gen_code_head();
  void gen_code();
  void gen_data();
};

#endif
