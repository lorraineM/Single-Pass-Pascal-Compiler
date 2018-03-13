// #define SCAN_DEBUG
// #define PARSE_DEBUG

#include "globals.h"
#include "pascal.tab.h"

extern FILE* yyin;

FILE * source;  //input source code
FILE * listing = stdout; //used to output debug info

symboltable* st = new symboltable();//external symboltable operator

extern TreeNode * do_parse(void);
extern int yy_flex_debug;

int main( int argc, char * argv[] ){
  int i;
  char pgm[120]; /* source code file name */
  if (argc != 3){ 
    fprintf(stderr,"usage: %s <input> <output>\n",argv[0]);
    exit(1);
  }
  strcpy(pgm,argv[1]);
  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".pas");
  source = fopen(pgm,"r");
  if (source == NULL){
    fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }
  yyin = source;
  listing = stdout; /* send listing to screen */
  fprintf(listing,"\nPASCAL COMPILATION: %s\n",pgm);
  
  //scan begin
#ifdef SCAN_DEBUG
  yy_flex_debug = 1;
#endif

#ifdef PARSE_DEBUG
  yydebug = 1;
#endif
  
  auto syntaxTree = dynamic_cast<Program_Node*>(do_parse());

  st->global = 1;
  syntaxTree->build_symbol_table("");
  // puts("=========symtab debug============");
  // for (i=0; i<SIZE; i++) {
  //   if (st->units[i].use == 1) {
  //     puts("=======================");
  //     printf("name: %s", st->units[i].name.c_str()); puts("");
  //     printf("type: %s", st->units[i].type.c_str()); puts("");
  //     printf("array_start: %d", st->units[i].array_start); puts("");
  //     printf("volumn: %d", st->units[i].volumn); puts("");
  //     printf("isrecord: %d", st->units[i].volumn); puts("");
  //     printf("volumn: %d", st->units[i].volumn); puts("");
  //     printf("printer: %p\n", &(st->units[i]));
  //   }
  // }
  // puts("=========symtab debug  end========");

  /* code generation */
  CodeGenerator* cg = new CodeGenerator(syntaxTree, argv[2]);
  cg->generate();

  fclose(source);
  return 0;
}
