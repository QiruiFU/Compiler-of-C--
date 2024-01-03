#ifndef STACK
#define STACK

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

Stack* StackInit();
void StackPush(Stack* self, HashTable* table);
HashTable* StackTop(Stack* stack);
void StackPop(Stack* stack);
HashTableNode* StackFind(Stack* stack, const Symbol sym);
HashTableNode* StackTopFind(Stack* stack, const Symbol sym);

#endif
