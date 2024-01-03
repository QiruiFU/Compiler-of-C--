#ifndef STACK
#define STACK

#include "hash.h"
#include<stdlib.h>
#include<assert.h>

struct StackNode_def{
    HashTable* table_;
    struct StackNode_def* nxt;
    struct StackNode_def* prv;
};
typedef struct StackNode_def StackNode;

struct Stack_def{
    int size_;
    StackNode* top_;
};
typedef struct Stack_def Stack;

Stack* StackInit();
void StackPush(Stack* self, HashTable* table);
HashTable* StackTop(Stack* self);
void StackPop(Stack* self);
HashTableNode* StackFind(Stack* self, const Symbol sym);
HashTableNode* StackTopFind(Stack* self, const Symbol sym);

#endif
