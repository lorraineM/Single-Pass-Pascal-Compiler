CFLAGS = -std=c++11
CC = g++
objects = main.o scan.o parse.o symtab_build.o codegen.o codegen_node.o
AST_header = node/*.hpp


all:	pascal

install: pascal
	cp pascal ../

pascal:	$(objects)
	$(CC) $(CFLAGS) $(objects) -o pascal

main.o: main.c pascal.tab.h globals.h $(AST_header)
	$(CC) $(CFLAGS) -c main.c -o main.o

symtab_build.o: symtab/symtab_build.cpp symtab/symboltable.h $(AST_header)
	$(CC) $(CFLAGS) -c symtab/symtab_build.cpp -o symtab_build.o

codegen.o: codegen/codegen.cpp codegen/codegen.hpp
	$(CC) $(CFLAGS) -c codegen/codegen.cpp -o codegen.o
  
codegen_node.o: codegen/codegen_node.cpp codegen/codegen.hpp $(AST_header)
	$(CC) $(CFLAGS) -c codegen/codegen_node.cpp -o codegen_node.o

scan.c: flex/pascal.l pascal.tab.h
	flex -o scan.c --case-insensitive flex/pascal.l

parse.o: pascal.tab.c globals.h $(AST_header)
	$(CC) $(CFLAGS) -c pascal.tab.c -o parse.o 

pascal.tab.c pascal.tab.h: bison/pascal.y
	bison --debug -d -b pascal bison/pascal.y

clean: 
	rm -f *.o pascal pascal.tab.c pascal.tab.h scan.c *.out
