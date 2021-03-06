#include "hashtable.h"

HashTable::HashTable()
{
	memset(table, 0, sizeof(table));
}

/*
	If hashtable is full nothing is inserted.
*/
void HashTable::insert(char *key, void *value)
{
//	printf("HashTable::insert(%s, %p)\n", key, value);
	int index;

	for(int i = 0; i < TABLE_SIZE; i++)
	{
		index = hash(key, i);

		if (table[index] == NULL)
		{
			node_t *node = new node_t;
			node->key = key;
			node->value = value;
			table[index] = node;
			break;
		}
	}
}

bool HashTable::update(const char *key, void *value)
{
//	printf("HashTable::update(%s, %s)\n", key, value);
	int index;

	for(int i = 0; i < TABLE_SIZE; i++)
	{
		index = hash(key, i);

		if (table[index] == NULL)
			continue;

		if (strcmp(table[index]->key, key) == 0)
		{
			table[index]->value = value;
			return true;
		}
	}
	return false;
}


void *HashTable::find(const char *key) const
{
//	printf("HashTable::find(%s)\n", key);
	int index;

	for(int i = 0; i < TABLE_SIZE; i++)
	{
		index = hash(key, i);

		// tried to find a key that doesnt exist
		if (table[index] == NULL)
			return NULL;

		if (strcmp(key, table[index]->key) == 0)
			return table[index]->value;
	}

	return (void *)NULL;
}

/*
		CLRS: h2 must be 'relatively prime' for entire table to be searched
	To ensure this let m be a power of two and h2 odd or let m be prime and
	let h2 be less than m. (m = table size)
*/
int HashTable::hash(const char *key, int i) const
{
	unsigned int h1 = hash_djb2(key);
	unsigned int h2 = hash_sdbm(key);

	// make h2 odd
	if (h2 % 2 == 0)
		h2 += 1;

	return ( h1 + (i * h2) ) % TABLE_SIZE;
}


int HashTable::hash_djb2(const char *key) const
{
	unsigned int hashval = 5381;
	int c;

	while (c = *key++)
		hashval = ((hashval << 5) + hashval) + c;

	return hashval;
}


int HashTable::hash_sdbm(const char *key) const
{
	unsigned int hashval = 0;
	int c;

	while (c = *key++)
		hashval = c + (hashval << 6) + (hashval << 16) - hashval;

	return hashval;
}
