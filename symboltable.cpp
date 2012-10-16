#include "symboltable.h"

SymbolTable::SymbolTable()
{
	push();
}

SymbolTable::~SymbolTable()
{
	while (stack.size())
		pop();
}

void SymbolTable::push()
{
	HashTable *hash_table = new HashTable;

	stack.push_back(hash_table);
}

void SymbolTable::pop()
{
	// delete symbol table
	delete stack[stack.size() - 1];

	// remove symbol pointer from stack
	stack.erase(stack.begin() + stack.size() - 1);
}

bool SymbolTable::global()
{
	if (stack.size() - 1 == 0)
		return true;
	else
		return false;
}

void SymbolTable::insert(char *key, void *value)
{
	stack[stack.size() - 1]->insert(key, value);
}

bool SymbolTable::update(const char *key, void *value)
{
	return stack[stack.size() - 1]->update(key, value);
}

void *SymbolTable::find(const char *key) const
{
	for(int i = stack.size() - 1; i >= 0; i--)
	{
		void *value = stack[i]->find(key);

		if (value != NULL)
			return value;
	}
	return NULL;
}

