// #ifndef HASH
// #define HASH

// #include "type_func.h"

// struct HashTableNode_def{
//     Symbol symbol;
//     struct HashTableNode_def* nxt;
// };
// typedef struct HashTableNode_def HashTableNode;

// typedef struct{
//     HashTableNode* table[16384];
// }HashTable;

// unsigned int Hash_func(char* str);
// HashTable* Hash_Init();
// void Hash_Add(HashTable* table, Symbol sym);
// HashTableNode* Hash_Find(HashTable* table, Symbol sym);

// #endif