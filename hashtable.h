#include "include.h"

#ifndef HASHTABLE_H
#define HASHTABLE_H


typedef struct
{
	char *key;
	void *value;
} node_t;

#define TABLE_SIZE 8

class HashTable
{
public:
	HashTable();
	void insert(char *key, void *value);
	void *find(const char *key) const;
	bool update(const char *key, void *value);
private:
	int hash(const char *key, int i) const;
	int hash_djb2(const char *key) const;
	int hash_sdbm(const char *key) const;
	node_t *table[TABLE_SIZE];
};

#endif
