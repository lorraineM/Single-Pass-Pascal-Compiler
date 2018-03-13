#include "codegen.hpp"

void CodeGenerator::generate()
{
#ifdef DEBUG
  printf("========== code generation debug ==========");
#endif
  syntax_tree->gen_code(this);
  this->gen_code_head();
  this->gen_code();
  this->gen_data();
}

void CodeGenerator::gen_code_head()
{
  fprintf(code_file, "# Simple Pascal Compiler for Compiler System Design Course.\n");
  fprintf(code_file, "# This is generated mips file from input pascal source file.\n");
  fprintf(code_file, "# To run this file, please use: \"spim -file [filename]\".\n");
  fprintf(code_file, ".text\n");
  fprintf(code_file, ".globl main\n");
}

void CodeGenerator::gen_code()
{
  for (int i = this->code_blocks.size() - 1; i >= 0; i--)
    fprintf(code_file, "\n%s", code_blocks[i]->generated_code().c_str());
  fprintf(code_file, "  li $v0, 10\n");
  fprintf(code_file, "  syscall\n");
}

void CodeGenerator::gen_data()
{
  fprintf(code_file, "\n.data\n");
  fprintf(code_file, "  __newline__: .asciiz \"\\n\"\n");
  for (int i = 0; i < this->data_blocks.size(); i++)
    fprintf(code_file, "%s", data_blocks[i]->generated_data().c_str());
}
