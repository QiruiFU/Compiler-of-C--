#ifndef HASH
#define HASH

#include "type-symbol.h"

struct HashTableNode_def{
    Symbol symbol;
    struct HashTableNode_def* nxt;
};
typedef struct HashTableNode_def HashTableNode;

typedef struct{
    HashTableNode* table[16384];
}HashTable;

HashTable* HashInit();
void HashAdd(HashTable* table, Symbol sym);
HashTableNode* HashFind(HashTable* table, Symbol sym);

#endif