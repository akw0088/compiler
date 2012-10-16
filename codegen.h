#include "include.h"

#ifndef CODEGEN_H
#define CODEGEN_H

/*
	This is just a class to hold all my global crap in gram.y
	not a good example of OOP :(
	but does allow me to test using the magical power of the visual studio debugger :D
*/

#define A_SYMBOL	1
#define A_GLOBAL	2
#define A_ARRAY		4
#define A_STACK		8

#define B_SYMBOL	16
#define B_GLOBAL	32
#define B_ARRAY		64
#define B_STACK		128

class CodeGen
{
public:
	CodeGen()
	{
		stack_offset = 4; // zero is ebp
		param_offset = 8; // zero is ebp, +4 is eip
		data_offset = 0;
		param = 0;
		expcount = 0;
		strcount = 0;
		data_skipped = 0;
		data_segment[0] = '\0';
		
	}

	void allocate(char *type, char *name, char *num);
	void allocate_param(char *type, char *name, char *num);
	char *allocatestr(char *str);
	void endallocate();
	void endallocate_param();
	void assignment(char *a, char *b);
	void addsub(char *op, char *a, char *b);
	void cmp(char *op, char *a, char *b);
	void muldiv(char *op, char *a, char *b);
	void exponential(char *a, char *b);
	void startloop(char *start, char *increment);
	void midloop(char *label, char *loopvar);
	void endloop(char *id, char *end);
	void conditional(char *a, char *op, char *b, int labelnum, bool final);
	

	void funccall(char *func);
	void push(char *operand);
	void startfunc(char *funcname);
	void endfunc();
	void ifblock(int labelnum);
	void elseblock(int labelnum);
	void endblock(int labelnum);

	void handle_array(char *name, char *index);
	void ret(char *retval);
	void data();

private:
	int param_offset;
	int stack_offset;
	int data_offset;
	int data_skipped;
	int param;
	int expcount;
	int strcount;

	char data_segment[4096];
};

#endif
