#include "include.h"

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

class SymbolTable
{
public:
	SymbolTable();
	void push();
	void pop();
	void insert(char *key, void *value);
	bool update(const char *key, void *value);
	bool global();
	void *find(const char *key) const;
	~SymbolTable();
private:
	std::vector< HashTable * > stack;
};

#endif
