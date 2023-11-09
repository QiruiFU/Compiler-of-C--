#ifndef STACK
#define STACK

#include "config.h"
#include "Hash.h"
#include<stdlib.h>
#include<assert.h>

struct StackNode_def{
    HashTable* table;
    struct StackNode_def* nxt;
    struct StackNode_def* prv;
};
typedef struct StackNode_def StackNode;

struct Stack_def{
    int size;
    StackNode* top;
};
typedef struct Stack_def Stack;

Stack* Stack_init();
void Stack_push(Stack* stack, HashTable* table);
HashTable* Stack_top(Stack* stack);
void Stack_pop(Stack* stack);
HashTableNode* Stack_find(Stack* stack, const Symbol sym);
HashTableNode* Stack_top_find(Stack* stack, const Symbol sym);

#endif
