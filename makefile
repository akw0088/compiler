compiler	:gram.o lex.o symboltable.o hashtable.o codegen.o
		g++ gram.o lex.o symboltable.o hashtable.o codegen.o -o compiler

symboltable.o	: symboltable.cpp
		g++ -c symboltable.cpp

hashtable.o	: hashtable.cpp
		g++ -c hashtable.cpp

codegen.o	: codegen.cpp
		g++ -c codegen.cpp

gram.o		: gram.c
		g++ -c -g gram.c

gram.c		: gram.y
		yacc -dv gram.y
		cp y.tab.c gram.c

lex.o		: gram.y lex.c
		g++ -c -g lex.c

lex.c		: lex.l
		flex lex.l
		cp lex.yy.c lex.c

clean		:
		rm compiler *.o gram.c lex.c y.tab.h y.tab.c y.output lex.yy.c
