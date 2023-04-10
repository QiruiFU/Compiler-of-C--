#ifndef HASH
#define HASH

#include "type.h"

typedef struct{
    Symbol symbol;
    HashTableNode* nxt;
}HashTableNode;

typedef struct{
    HashTableNode* table[16384] = {0};
}HashTable;

unsigned int Hash_func(char* str);
void Hash_Add(HashTable* table, Symbol sym);
bool Hash_Find(HashTable* table, Symbol sym);

#endif