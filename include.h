#ifndef INCLUDE_H
#define INCLUDE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "hashtable.h"
#include "symboltable.h"
#include "codegen.h"

typedef struct
{
	int offset;
	int type;
	bool global;
	bool function;
	int size;

	// supporting arrays of depth 3, general method not immediately apparent
	int num_dimensions;
	int dimension[3];
	int current_dimension;
} symbol_t;

#endif
