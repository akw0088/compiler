#include "codegen.h"

extern SymbolTable symbol_table;
int symbol_flags(symbol_t *a, symbol_t *b);

/*
	Allocates stack space on a per item basis
*/
void CodeGen::allocate(char *type, char *name, char *dimensions)
{
	int		type_size = 4;
	int		stack_size = 0;
	symbol_t	*symbol = new symbol_t;

	symbol->current_dimension = 0;

	if ( strcmp(type, "char") == 0)
	{
		type_size = 1;
		symbol->type = 0;
	}
	else if( strcmp(type, "int") == 0)
	{
		symbol->type = 1;
		symbol->size = 4;
	}
	else if( strcmp(type, "void") == 0)
	{
		symbol->type = 2;
		symbol->size = 4;
	}
	else
	{
		//boolean
		symbol->type = 3;
		symbol->size = 4;
	}

	if (symbol_table.global())
	{
		symbol->global = true;
		symbol->offset = data_offset;
	}
	else
	{
		symbol->global = false;

		//negatives ensure we go right direction on the stack
		symbol->offset = -stack_offset;
		symbol->size = -symbol->size;
	}

	symbol->num_dimensions = 0;
	symbol->dimension[0] = 0;
	symbol->dimension[1] = 0;
	symbol->dimension[2] = 0;
	stack_size = 4;


	if (dimensions != NULL)
	{
		char	x[80], y[80], z[80];

		switch (sscanf(dimensions, "%s %s %s", x, y, z))
		{
		case 1:
			symbol->num_dimensions = 1;
			symbol->dimension[0] = atoi(x);
			stack_size = symbol->dimension[0] * abs(symbol->size);
			break;
		case 2:
			symbol->num_dimensions = 2;
			symbol->dimension[0] = atoi(x);
			symbol->dimension[1] = atoi(y);
			stack_size = symbol->dimension[0] * symbol->dimension[1] * abs(symbol->size);
			break;
		case 3:
			symbol->num_dimensions = 3;
			symbol->dimension[0] = atoi(x);
			symbol->dimension[1] = atoi(y);
			symbol->dimension[2] = atoi(z);
			stack_size = symbol->dimension[0] * symbol->dimension[1] * symbol->dimension[2] * abs(symbol->size);
			break;
		default:
			fprintf(stderr, "Unexpected dimension string: %s\n", dimensions);
		}
	}

	symbol_table.insert(name, (void *)symbol);
	if (symbol_table.global())
	{
		data_offset += stack_size;
	}
	else
	{
		stack_offset += stack_size;
	}
}

void CodeGen::allocate_param(char *type, char *name, char *dimensions)
{
//	printf("CodeGen::allocate_param(%s,%s,%s)\n", type, name, dimensions);
	int		type_size = 4;
	int		stack_size = 0;
	symbol_t	*symbol = new symbol_t;

	// Init symbol
	symbol->current_dimension = 0;
	if ( strcmp(type, "char") == 0)
	{
		type_size = 1;
		symbol->type = 0;
	}
	else if( strcmp(type, "int") == 0)
	{
		symbol->type = 1;
		symbol->size = 4;
	}
	else if( strcmp(type, "void") == 0)
	{
		symbol->type = 2;
		symbol->size = 4;
	}
	else
	{
		//boolean
		symbol->type = 3;
		symbol->size = 4;
	}
	stack_size = symbol->size;
	symbol->global = false;
	symbol->offset = param_offset;
	symbol->size = -symbol->size;
	symbol->num_dimensions = 0;
	symbol->dimension[0] = 0;
	symbol->dimension[1] = 0;
	symbol->dimension[2] = 0;


	//determine array size
	if (dimensions != NULL)
	{
		char	x[80], y[80], z[80];

		switch (sscanf(dimensions, "%s %s %s", x, y, z))
		{
		case 1:
			symbol->num_dimensions = 1;
			symbol->dimension[0] = atoi(x);
			stack_size = symbol->dimension[0] * abs(symbol->size);
			break;
		case 2:
			symbol->num_dimensions = 2;
			symbol->dimension[0] = atoi(x);
			symbol->dimension[1] = atoi(y);
			stack_size = symbol->dimension[0] * symbol->dimension[1] * abs(symbol->size);
			break;
		case 3:
			symbol->num_dimensions = 3;
			symbol->dimension[0] = atoi(x);
			symbol->dimension[1] = atoi(y);
			symbol->dimension[2] = atoi(z);
			stack_size = symbol->dimension[0] * symbol->dimension[1] * symbol->dimension[2] * abs(symbol->size);
			break;
		default:
			fprintf(stderr, "Unexpected dimension string: %s\n", dimensions);
		}
	}

	symbol_table.insert(name, (void *)symbol);
	param_offset += stack_size;

}

/*
	To make this work right, we will have to put data segment after code segment
and initialize with assembler specific code
*/
char *CodeGen::allocatestr(char *str)
{
	char *temp = new char[80];
	char buffer[512];

	snprintf(temp, 80, "string%d", ++strcount);

	if (data_offset - data_skipped)
	{
		sprintf(buffer, ".skip %d\n", data_offset - data_skipped);
		strcat(data_segment, buffer);
	}

	sprintf(buffer, "%s: .ascii %s\n.byte 0\n", temp, str);
	strcat(data_segment, buffer);

	data_offset += strlen(str) + 1;
	data_skipped = data_offset;
	snprintf(temp, 80, "OFFSET string%d", strcount);
	return temp;
}

// outputs stack sub instruction for all locals
void CodeGen::endallocate()
{
	if (stack_offset > 4)
		printf("\tsub esp, %d\n", stack_offset);
}

// resets param offset
void CodeGen::endallocate_param()
{
	param_offset = 8;
}

// this theoretically can do mov and imul, but narrow scope makes generated code nice
void CodeGen::addsub(char *op, char *a, char *b)
{
//	printf("CodeGen::addsub(%s,%s,%s)\n", op, a, b);
	symbol_t *symbol_a = (symbol_t *)symbol_table.find(a);
	symbol_t *symbol_b = (symbol_t *)symbol_table.find(b);
	int type = symbol_flags(symbol_a, symbol_b);

	if ( strcmp(a, "stack") == 0 )
	{
		type |= A_STACK;
	}
	if ( strcmp(b, "stack") == 0 )
	{
		type |= B_STACK;
	}


	switch (type)
	{
	case 0:
// two constants
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, %s\n", op, b);
		printf("\tpush eax\n");
		break;
// b is constant
	case A_STACK:
		printf("\tpop eax\n");
		printf("\t%s eax, %s\n", op, b);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, %s\n", op, b);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, %s\n", op, b);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, %s\n", op, b);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, %s\n", op, b);
		printf("\tpush eax\n");
		break;

// a is constant
	case B_STACK:
		printf("\tpop ebx\n");
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, ebx\n", op);
		printf("\tpush eax\n");
		break;
	case B_SYMBOL:
		printf("\tmov ebx, [ebp%+d]\n", symbol_b->offset);
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, ebx\n", op);
		printf("\tpush eax\n");
		break;
	case B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, [ebp+edx%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, %s\n", a);
		printf("\tmov ebx, [data_segment%+d]\n", symbol_b->offset);
		printf("\t%s eax, ebx\n", op);
		printf("\tpush eax\n");
		break;
	case B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, [data_segment+edx%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;

// both on stack
	case A_STACK | B_STACK:
		printf("\tpop ebx\n");
		printf("\tpop eax\n");
		printf("\t%s eax, ebx\n", op);
		printf("\tpush eax\n");
	break;

// b is symbol
	case A_STACK | B_SYMBOL:
		printf("\tpop eax\n");
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | B_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;

// a is symbol
	case A_SYMBOL | B_STACK:
		printf("\tpop eax\n");
		printf("\t%s [ebp%+d], eax\n", op, symbol_a->offset);
		printf("\tpush DWORD PTR [ebp%+d]\n", symbol_a->offset);
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp+edx%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment+edx%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;

//b is array
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp+edx%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp+esi%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp+esi%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;

// a is array
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment+esi%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;

//b is global symbol
	case A_STACK | B_SYMBOL | B_GLOBAL:
		printf("\tpop eax");
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;

// a is global symbol
	case  A_SYMBOL | A_GLOBAL | B_STACK:
		printf("\tpop eax");
		printf("\t%s [data_segment%+d], eax\n", op, symbol_a->offset);
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment+edx%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;

//b is global array
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment+esi%+d]\n", op, symbol_b->offset);
		printf("\tpush eax\n");
		break;
	}
}

/*
	did this for cmp, same as subtraction but doesnt store result
	since results are stored on stack, this is exactly the same as
	addsub, but without the pushing of the result
*/
void CodeGen::cmp(char *op, char *a, char *b)
{
	symbol_t *symbol_a = (symbol_t *)symbol_table.find(a);
	symbol_t *symbol_b = (symbol_t *)symbol_table.find(b);
	int type = symbol_flags(symbol_a, symbol_b);

	if ( strcmp(a, "stack") == 0 )
	{
		type |= A_STACK;
	}
	if ( strcmp(b, "stack") == 0 )
	{
		type |= B_STACK;
	}


	switch (type)
	{
	case 0:
// two constants
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, %s\n", op, b);
		break;
// b is constant
	case A_STACK:
		printf("\tpop eax\n");
		printf("\t%s eax, %s\n", op, b);
		break;
	case A_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, %s\n", op, b);
		break;
	case A_SYMBOL | A_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, %s\n", op, b);
		break;
	case A_SYMBOL | A_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, %s\n", op, b);
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, %s\n", op, b);
		break;

// a is constant
	case B_STACK:
		printf("\tpop ebx\n");
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, ebx\n", op);
		break;
	case B_SYMBOL:
		printf("\tmov ebx, [ebp%+d]\n", symbol_b->offset);
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, ebx\n", op);
		break;
	case B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, [ebp+edx%+d]\n", op, symbol_b->offset);
		break;
	case B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, %s\n", a);
		printf("\tmov ebx, [data_segment%+d]\n", symbol_b->offset);
		printf("\t%s eax, ebx\n", op);
		break;
	case B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, %s\n", a);
		printf("\t%s eax, [data_segment+edx%+d]\n", op, symbol_b->offset);
		break;

// both on stack
	case A_STACK | B_STACK:
		printf("\tpop ebx\n");
		printf("\tpop eax\n");
		printf("\t%s eax, ebx\n", op);
	break;

// b is symbol
	case A_STACK | B_SYMBOL:
		printf("\tpop eax\n");
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | B_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp%+d]\n", op, symbol_b->offset);
		break;

// a is symbol
	case A_SYMBOL | B_STACK:
		printf("\tpop eax\n");
		printf("\t%s [ebp%+d], eax\n", op, symbol_a->offset);
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp+edx%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment+edx%+d]\n", op, symbol_b->offset);
		break;

//b is array
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp+edx%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp+esi%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [ebp+esi%+d]\n", op, symbol_b->offset);
		break;

// a is array
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment+esi%+d]\n", op, symbol_b->offset);
		break;

//b is global symbol
	case A_STACK | B_SYMBOL | B_GLOBAL:
		printf("\tpop eax");
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		break;

	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment%+d]\n", op, symbol_b->offset);
		break;

// a is global symbol
	case  A_SYMBOL | A_GLOBAL | B_STACK:
		printf("\tpop eax");
		printf("\t%s [data_segment%+d], eax\n", op, symbol_a->offset);
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment+edx%+d]\n", op, symbol_b->offset);
		break;

//b is global array
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s eax, [data_segment+esi%+d]\n", op, symbol_b->offset);
		break;
	}
}


void CodeGen::muldiv(char *op, char *a, char *b)
{
//	printf("CodeGen::muldiv(%s,%s,%s)\n", op, a, b);
	symbol_t *symbol_a = (symbol_t *)symbol_table.find(a);
	symbol_t *symbol_b = (symbol_t *)symbol_table.find(b);
	int type = symbol_flags(symbol_a, symbol_b);
	char *result = "\tpush eax\n";

	if ( strcmp(op, "mod") == 0 )
	{
		op = "idiv";
		result = "\tpush edx\n";
		printf("\tmov edx, 0\n"); // fixes overflow error
	}

	if ( strcmp(a, "stack") == 0 )
	{
		type |= A_STACK;
	}
	if ( strcmp(b, "stack") == 0 )
	{
		type |= B_STACK;
	}

	switch (type)
	{
	case 0:
// two constants
		printf("\tmov eax, %s\n", a);
		printf("\tmov ebx, %s\n", b);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;
// b is constant
	case A_STACK:
		printf("\tpop eax\n");
		printf("\tmov ebx, %s\n", b);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;
	case A_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\tmov ebx, %s\n", b);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;
	case A_SYMBOL | A_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\tmov ebx, %s", b);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;
	case A_SYMBOL | A_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\tmov ebx, %s\n", b);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\tmov ebx, %s", b);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;

// a is constant
	case B_STACK:
		printf("\tpop ebx\n");
		printf("\tmov eax, %s\n", a);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;
	case B_SYMBOL:
		printf("\tmov ebx, [ebp%+d]\n", symbol_b->offset);
		printf("\tmov eax, %s\n", a);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;
	case B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, %s\n", a);
		printf("\t%s DWORD PTR [ebp+edx%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, %s\n", a);
		printf("\tmov ebx, DWORD PTR [data_segment%+d]\n", symbol_b->offset);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;
	case B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, %s\n", a);
		printf("\t%s DWORD PTR [data_segment+edx%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;

// both on stack
	case A_STACK | B_STACK:
		printf("\tpop ebx\n");
		printf("\tpop eax\n");
		printf("\t%s ebx\n", op);
		printf("%s", result);
	break;

// b is symbol
	case A_STACK | B_SYMBOL:
		printf("\tpop eax\n");
		printf("\t%s DWORD PTR [ebp%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | B_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [ebp%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [ebp%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [ebp%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [ebp%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;

// a is symbol
	case A_SYMBOL | B_STACK:
		printf("\tpop ebx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s ebx\n", op);
		printf("%s", result);
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [data_segment%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [ebp+edx%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [data_segment+edx%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;

//b is array
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [ebp+edx%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [ebp+esi%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [ebp+esi%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;

// a is array
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [data_segment%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [data_segment+esi%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;

//b is global symbol
	case A_STACK | B_SYMBOL | B_GLOBAL:
		printf("\tpop eax");
		printf("\t%s DWORD PTR [data_segment%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [data_segment%+d]\n", op, symbol_b->offset);
		printf("%s", result);
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [data_segment%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;

// a is global symbol
	case  A_SYMBOL | A_GLOBAL | B_STACK:
		printf("\tpop ebx");
		printf("\tmov eax, [data_segment%+d]", symbol_a->offset);
		printf("\t%s , ebx\n", op);
		printf("%s", result);
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [data_segment+edx%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;

//b is global array
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_a->offset);
		printf("\t%s DWORD PTR [data_segment+esi%+d]\n", op, symbol_b->offset);
		printf("%s", result);
		break;
	}
}

void CodeGen::exponential(char *a, char *b)
{
//	printf("CodeGen::exponential(%s,%s)\n", a, b);
	symbol_t *symbol_a = (symbol_t *)symbol_table.find(a);
	symbol_t *symbol_b = (symbol_t *)symbol_table.find(b);
	int type = symbol_flags(symbol_a, symbol_b);
	char label[80];

	snprintf(label, 80, "EXP%d", ++expcount);

	if ( strcmp(a, "stack") == 0 )
	{
		type |= A_STACK;
	}
	if ( strcmp(b, "stack") == 0 )
	{
		type |= B_STACK;
	}


	switch (type)
	{
	case 0:
// two constants
		printf("\tmov eax, %s\n", a);
		printf("\tpush ecx\n");
		printf("\tmov ecx, %d\n", atoi(b) - 1);
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
// b is constant
	case A_STACK:
		printf("\tpop eax\n");
		printf("\tpush ecx\n");
		printf("\tmov ecx, %d\n", atoi(b) - 1);
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\tpush ecx\n");
		printf("\tmov ecx, %d\n", atoi(b) - 1);
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_ARRAY:
		break;
	case A_SYMBOL | A_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\tpush ecx\n");
		printf("\tmov ecx, %d\n", atoi(b) - 1);
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY:
		break;

// a is constant
	case B_STACK:
		printf("\tmov eax, %s\n", a);
		printf("\tpush ecx\n");
		printf("\tpop ecx");
		printf("\tdec ecx\n");
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case B_SYMBOL:
		printf("\tmov eax, %s\n", a);
		printf("\tpush ecx\n");
		printf("\tmov ecx, [ebp%+d]\n", symbol_b->offset);
		printf("\tdec ecx\n");
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case B_SYMBOL | B_ARRAY:
		break;
	case B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, %s\n", a);
		printf("\tpush ecx\n");
		printf("\tmov ecx, [data_segment%+d]\n", symbol_b->offset);
		printf("\tdec ecx\n");
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case B_SYMBOL | B_GLOBAL | B_ARRAY:
		break;

// both on stack
	case A_STACK | B_STACK:
		printf("\tpop edx\n");
		printf("\tpop eax\n");
		printf("\tpush ecx\n");
		printf("\tmov ecx, edx\n");
		printf("\tdec ecx\n");
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
	break;

// b is symbol
	case A_STACK | B_SYMBOL:
		printf("\tpop eax, a\n");
		printf("\tpush ecx\n");
		printf("\tmov ecx, [ebp%+d]\n", symbol_b->offset);
		printf("\tdec ecx\n");
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | B_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\tpush ecx\n");
		printf("\tmov ecx, [ebp%+d]\n", symbol_b->offset);
		printf("\tdec ecx\n");
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\tpush ecx\n");
		printf("\tmov ecx, [ebp%+d]\n", symbol_b->offset);
		printf("\tdec ecx\n");
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL:
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL:
		break;

// a is symbol
	case A_SYMBOL | B_STACK:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\tpop edx\n");
		printf("\tpush ecx\n");
		printf("\tmov ecx, edx\n");
		printf("\tdec ecx\n");
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [ebp%+d]\n", symbol_a->offset);
		printf("\tpush ecx\n");
		printf("\tmov ecx, [data_segment%+d]\n", symbol_b->offset);
		printf("\tdec ecx\n");
		printf("\tmov ebx, eax\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | B_SYMBOL | B_ARRAY:
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		break;

//b is array
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_ARRAY:
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_ARRAY:
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_ARRAY:
		break;

// a is array
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		break;

//b is global symbol
	case A_STACK | B_SYMBOL | B_GLOBAL:
		printf("\tpop eax");
		printf("\tpush ecx\n");
		printf("\tmov ecx, [data_segment%+d]\n", symbol_b->offset);
		printf("\tdec ecx\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\tpush ecx\n");
		printf("\tmov ecx, [data_segment%+d]\n", symbol_b->offset);
		printf("\tdec ecx\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		break;

// a is global symbol
	case  A_SYMBOL | A_GLOBAL | B_STACK:
		printf("\tmov eax, [data_segment%+d]\n", symbol_a->offset);
		printf("\tpop edx\n");
		printf("\tpush ecx\n");
		printf("\tmov ecx, edx");
		printf("\tdec ecx\n");
		printf("%s:\n", label);
		printf("\tmul ebx\n");
		printf("\tsub ecx, 1\n");
		printf("\tjne %s\n", label);
		printf("\tpop ecx\n");
		printf("\tpush eax\n");
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		break;

//b is global array
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		break;
	}
}

/*
	Sets up registers before loop label
	ecx - loop variable
	esi - loop increment
*/
void CodeGen::startloop(char *start, char *increment)
{
	symbol_t *symbol_start = (symbol_t *)symbol_table.find(start);
	symbol_t *symbol_increment = (symbol_t *)symbol_table.find(increment);
	int type_start = symbol_flags(symbol_start, NULL);
	int type_increment = symbol_flags(symbol_increment, NULL);

	if ( strcmp(start, "stack") == 0 )
	{
		type_start |= A_STACK;
	}
	if ( strcmp(increment, "stack") == 0 )
	{
		type_increment |= A_STACK;
	}

	printf("\tpush ecx\n");
	printf("\tpush edi\n");

	switch(type_start)
	{
	case 0:
		printf("\tmov ecx, %s\n", start);
		break;
	case A_SYMBOL:
		printf("\tmov ecx, [ebp%+d]\n", symbol_start->offset);
		break;
	case A_SYMBOL | A_GLOBAL:
		printf("\tmov ecx, [data_segment%+d]\n", symbol_start->offset);
		break;
	case A_STACK:
		printf("\tpop ecx\n");
		break;
	}

	switch(type_increment)
	{
	case 0:
		printf("\tmov edi, %s\n", increment);
		break;
	case A_SYMBOL:
		printf("\tmov edi, [ebp%+d]\n", symbol_increment->offset);
		break;
	case A_SYMBOL | A_GLOBAL:
		printf("\tmov edi, [data_segment%+d]\n", symbol_increment->offset);
		break;
	case A_STACK:
		printf("\tpop edi\n");
		break;
	}
}

/*
	outputs label and updates loop variable with current loop value (i = i++)
*/
void CodeGen::midloop(char *label, char *loopvar)
{
	symbol_t *symbol = (symbol_t *)symbol_table.find(loopvar);

	printf("LOOP_%s:\n", label);
	printf("\tmov [ebp%+d], ecx\n", symbol->offset);
}

/*
	increments loop var, compare, jump
*/
void CodeGen::endloop(char *id, char *end)
{
//	printf("CodeGen::endloop(%s,%s)\n", id, end);
	symbol_t *symbol_end = (symbol_t *)symbol_table.find(end);
	int type = symbol_flags(symbol_end, NULL);

	if ( strcmp(end, "stack") == 0 )
	{
		type |= A_STACK;
	}

	printf("\tadd ecx, edi\n");

	switch(type)
	{
	case 0:
		printf("\tcmp ecx, %s\n", end);
		break;
	case A_SYMBOL:
		printf("\tcmp ecx, [ebp%+d]\n", symbol_end->offset);
		break;
	case A_SYMBOL | A_GLOBAL:
		printf("\tcmp ecx, [data_segment%+d]\n", symbol_end->offset);
		break;
	case A_STACK:
		printf("\tpop esi\n");
		printf("\tpop edi\n");
		printf("\tpop eax\n");
		printf("\tpush edi\n");
		printf("\tpush esi\n");
		printf("\tcmp ecx, eax\n");
		break;
	}

	printf("\tjle LOOP_%d\n", atoi(id));
	printf("\tpop edi\n");
	printf("\tpop ecx\n\n");
}

/*
	compares and jumps (Shouldnt jump, but does)
*/
void CodeGen::conditional(char *a, char *op, char *b, int labelnum, bool final)
{
//	printf("CodeGen::conditional(%s,%s,%s,%d)\n", a, op, b, final);
	symbol_t *symbol_a = (symbol_t *)symbol_table.find(a);
	symbol_t *symbol_b = (symbol_t *)symbol_table.find(b);

	cmp("cmp", a, b);
	/*
		We fall through to true code and jump to false code.
		gcc jumps to false code, we cant do that due to LR single pass
	*/
	if (strcmp(op, "==") == 0)
		printf("\tjne LABEL_ELSE%d\n", labelnum);
	else if (strcmp(op, "!=") == 0)
		printf("\tje LABEL_ELSE%d\n", labelnum);
	else if (strcmp(op, ">=") == 0)
		printf("\tjl LABEL_ELSE%d\n", labelnum);
	else if (strcmp(op, "<=") == 0)
		printf("\tjg LABEL_ELSE%d\n", labelnum);
	else if (strcmp(op, ">") == 0)
		printf("\tjle LABEL_ELSE%d\n", labelnum);
	else if (strcmp(op, "<") == 0)
		printf("\tjge LABEL_ELSE%d\n", labelnum);

	if (final)
	{
		printf("LABEL_IF%d:\n", labelnum);
	}
}


/*
	function returns value in eax, popad obliterates that value
*/
void CodeGen::funccall(char *func)
{
//	printf("CodeGen::funccall(%s)\n", func);

	if (param == 0)
	{
		printf("\tpushad\n");
	}
	printf("\tcall %s\n", func);
	if (param)
	{
		printf("\tadd esp, %d\n", 4 * param);
	}
	printf("\tpopad\n");
	param = 0;
}

void CodeGen::push(char *operand)
{
//	printf("CodeGen::push(%s)\n", operand);
	symbol_t *symbol = (symbol_t *)symbol_table.find(operand);

	if (symbol == NULL)
	{
		if (strcmp(operand, "stack") == 0)
		{
			if (param == 0)
			{
				printf("\tpop eax\n");
				printf("\tpushad\n");
				printf("\tpush eax\n");
			}

			param++;
			return;
		}

		printf("\tmov eax, %s\n", operand);
	}
	else
	{
		if (symbol->num_dimensions == 0)
		{

			if (symbol->global)
			{
				printf("\tmov eax, [data_segment%+d]\n", symbol->offset);
			}
			else
			{
				printf("\tmov eax, [ebp%+d]\n", symbol->offset);
			}
		}
		else
		{
			if (symbol->global)
			{
				printf("\tpop edx\n");
				printf("\tmov eax, [data_segment+edx%+d]\n", symbol->offset);
			}
			else
			{
				printf("\tpop edx\n");
				printf("\tmov eax, [ebp+edx%+d]\n", symbol->offset);
			}
		}
	}

	if (param == 0)
	{
		printf("\tpushad\n");
	}

	printf("\tpush eax\n");
	param++;
}

void CodeGen::startfunc(char *funcname)
{
	symbol_t symbol;
	symbol.num_dimensions = 0;
	symbol.global = false;
	symbol.offset = 0;
	symbol.size = 0;
	symbol.type = 0;
	symbol.function = true;
	symbol_table.insert(funcname, &symbol);

	stack_offset = 4;
	printf("\n%s:\n", funcname);
	printf("\tpush ebp\n");
	printf("\tmov ebp, esp\n");
}

void CodeGen::endfunc()
{
	printf("\tmov esp, ebp\n");
	printf("\tpop ebp\n");
	printf("\tret\n");
}

void CodeGen::elseblock(int labelnum)
{
	printf("\tjmp LABEL_ENDIF%d\n", labelnum);
	printf("LABEL_ELSE%d:\n", labelnum);
}

void CodeGen::ifblock(int labelnum)
{
	printf("LABEL_ELSE%d:\n", labelnum);
}

void CodeGen::endblock(int labelnum)
{
	printf("LABEL_ENDIF%d:\n", labelnum);
}

//index is either stack, a symbol, or a constant
/*
	Moves offset of array in bytes onto stack
	edx = edx + index * size
*/
void CodeGen::handle_array(char *name, char *index)
{
	symbol_t *symbol_name = (symbol_t *)symbol_table.find(name);
	symbol_t *symbol_index = (symbol_t *)symbol_table.find(index);

	if (symbol_name->current_dimension == symbol_name->num_dimensions)
		symbol_name->current_dimension = 0;

	symbol_name->current_dimension++;

//	printf("CodeGen::handle_array(%s,%s,%d)\n", name, index, symbol_name->current_dimension);

	if (strcmp(index, "stack") == 0)
	{
		printf("\tpop eax\n");
		printf("\tmov ebx, %d\n", symbol_name->size);
		printf("\timul ebx\n");
		printf("\tpush eax\n");
		if (symbol_name->current_dimension > 1)
		{
			printf("\tpop esi\n");
			printf("\tpop eax\n");
			printf("\tmov ebx, %d\n", symbol_name->dimension[symbol_name->current_dimension - 1]);
			printf("\timul ebx\n");
			printf("\tadd eax, esi\n");
			printf("\tpush eax\n");
		}
		return;
	}

	if (symbol_index == NULL)
	{
		printf("\tmov eax, %d\n", atoi(index) * symbol_name->size);
		printf("\tpush eax\n");
	}
	else
	{
		if (symbol_index->num_dimensions)
		{
			if (symbol_index->global)
			{
				printf("\tpop edx\n");
				printf("\tmov eax, [data_segment+edx%+d]\n", symbol_index->offset);
				printf("\tmov ebx, %d\n", symbol_name->size);
				printf("\timul ebx\n");
				printf("\tpush eax\n");
			}
			else
			{
				printf("\tpop edx\n");
				printf("\tmov eax, [ebp+edx%+d]\n", symbol_index->offset);
				printf("\tmov ebx, %d\n", symbol_name->size);
				printf("\timul ebx\n");
				printf("\tpush eax\n");
			}
		}
		else
		{
			if (symbol_index->global)
			{
				printf("\tmov eax, [data_segment%+d]\n", symbol_index->offset);
				printf("\tmov ebx, %d\n", symbol_name->size);
				printf("\timul ebx\n");
				printf("\tpush eax\n");
			}
			else
			{
				printf("\tmov eax, [ebp%+d]\n", symbol_index->offset);
				printf("\tmov ebx, %d\n", symbol_name->size);
				printf("\timul ebx\n");
				printf("\tpush eax\n");
			}
		}
	}

	//crappy bandaide for multidimensional arrays
	if (symbol_name->current_dimension > 1)
	{
		printf("\tpop esi\n");
		printf("\tpop eax\n");
		printf("\tmov ebx, %d\n", symbol_name->dimension[symbol_name->current_dimension - 1]);
		printf("\timul ebx\n");
		printf("\tadd eax, esi\n");
		printf("\tpush eax\n");
	}
}

/*
	looks up operands and performs assignment
	notice that operand lookup code is duplicated on all instructions
*/
void CodeGen::assignment(char *a, char *b)
{
//	printf("Codegen::assignment(%s,%s)\n", a, b);
	symbol_t *symbol_a = (symbol_t *)symbol_table.find(a);
	symbol_t *symbol_b = (symbol_t *)symbol_table.find(b);
	int type = symbol_flags(symbol_a, symbol_b);

	if ( strcmp(a, "stack") == 0 )
	{
		type |= A_STACK;
	}
	if ( strcmp(b, "stack") == 0 )
	{
		type |= B_STACK;
	}

	switch (type)
	{
	case 0:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
		break;
// b is constant
	case A_STACK:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
		break;
	case A_SYMBOL:
		printf("\tmov [ebp%+d], DWORD PTR %d\n", symbol_a->offset, atoi(b));
		break;
	case A_SYMBOL | A_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov [ebp+edx%+d], DWORD PTR %d\n", symbol_a->offset, atoi(b));
		break;
	case A_SYMBOL | A_GLOBAL:
		printf("\tmov [data_segment%+d], DWORD PTR %d\n", symbol_a->offset, atoi(b));
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov [data_segment+edx%+d], DWORD PTR %d\n", symbol_a->offset, atoi(b));
		break;

// a is constant
	case B_STACK:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
		break;
	case B_SYMBOL:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
		break;
	case B_SYMBOL | B_ARRAY:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
		break;
	case B_SYMBOL | B_GLOBAL:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
		break;
	case B_SYMBOL | B_GLOBAL | B_ARRAY:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
		break;

// both on stack
	case A_STACK | B_STACK:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
	break;

// b is symbol
	case A_STACK | B_SYMBOL:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
		break;
	case A_SYMBOL | B_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_b->offset);
		printf("\tmov [ebp%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL:
		printf("\tmov eax, [ebp%+d]\n", symbol_b->offset);
		printf("\tmov [data_segment%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_b->offset);
		printf("\tmov [ebp+edx%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_b->offset);
		printf("\tmov [data_segment+edx%+d], eax\n", symbol_a->offset);
		break;

// a is symbol
	case A_SYMBOL | B_STACK:
		printf("\tpop eax\n");
		printf("\tmov [ebp%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_b->offset);
		printf("\tmov [ebp%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+edx%+d]\n", symbol_b->offset);
		printf("\tmov [ebp%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_b->offset);
		printf("\tmov [ebp%+d], eax\n", symbol_a->offset);
		break;

//b is array
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp%+d]\n", symbol_b->offset);
		printf("\tmov [data_segment+edx%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+esi%+d]\n", symbol_b->offset);
		printf("\tmov [ebp+edx%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [ebp+esi%+d]\n", symbol_b->offset);
		printf("\tmov [data_segment+edx%+d], eax\n", symbol_a->offset);
		break;

// a is array
	case A_SYMBOL | A_ARRAY | B_STACK:
		printf("\tpop eax\n");
		printf("\tpop edx\n");
		printf("\tmov [ebp+edx%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_STACK:
		printf("\tpop eax\n");
		printf("\tpop edx\n");
		printf("\tmov [data_segment+edx%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment%+d]\n", symbol_b->offset);
		printf("\tmov [ebp+edx%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+esi%+d]\n", symbol_b->offset);
		printf("\tmov [ebp+edx%+d], eax\n", symbol_a->offset);
		break;

//b is global symbol
	case A_STACK | B_SYMBOL | B_GLOBAL:
		fprintf(stderr, "Invalid L-Value: mov %s %s\n", a, b);
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL:
		printf("\tmov eax, [data_segment%+d]\n", symbol_b->offset);
		printf("\tmov [data_segment%+d], eax\n", symbol_a->offset);
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment%+d]\n", symbol_b->offset);
		printf("\tmov [data_segment+edx%+d], eax\n", symbol_a->offset);
		break;

// a is global symbol
	case  A_SYMBOL | A_GLOBAL | B_STACK:
		printf("\tpop eax");
		printf("\tmov [data_segment%+d], eax\n", symbol_a->offset);
		break;
	case A_SYMBOL | A_GLOBAL | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+edx%+d]\n", symbol_b->offset);
		printf("\tmov [data_segment%+d], eax\n", symbol_a->offset);
		break;

//b is global array
	case A_SYMBOL | A_GLOBAL | A_ARRAY | B_SYMBOL | B_GLOBAL | B_ARRAY:
		printf("\tpop esi\n");
		printf("\tpop edx\n");
		printf("\tmov eax, [data_segment+esi%+d]\n", symbol_b->offset);
		printf("\tmov [data_segment+edx%+d], eax\n", symbol_a->offset);
		break;
	}
}

void CodeGen::ret(char *retval)
{
	symbol_t *symbol = (symbol_t *)symbol_table.find(retval);

	if (symbol == NULL)
	{
		printf("\tmov eax, %d\n", atoi(retval));
		return;
	}
	else if (symbol->global == false && symbol->num_dimensions == 0)
	{
		printf("\tmov eax, [ebp%+d]\n", symbol->offset);
		return;
	}
	else if (symbol->global == true && symbol->num_dimensions == 0)
	{
		printf("\tmov eax, [data_segment%+d]\n", symbol->offset);
		return;
	}
}

// emits data segment
void CodeGen::data()
{
	char buffer[80];

	if (data_offset - data_skipped)
	{
		sprintf(buffer, ".skip %d\n", data_offset - data_skipped);
		strcat(data_segment, buffer);
	}

	if (data_offset || data_segment[0])
	{
		printf(".data\n");
		printf("data_segment:\n%s\n", data_segment);
	}
}

int symbol_flags(symbol_t *a, symbol_t *b)
{
	int type = 0;

	if (a)
	{
		type |= A_SYMBOL;
		if (a->num_dimensions != 0)
		{
			type |= A_ARRAY;
		}

		if (a->global)
		{
			type |= A_GLOBAL;
		}
	}

	if (b)
	{
		type |= B_SYMBOL;
		if (b->num_dimensions != 0)
		{
			type |= B_ARRAY;
		}

		if (b->global)
		{
			type |= B_GLOBAL;
		}
	}

	return type;
}
