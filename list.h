#ifndef LIST
#define LIST

#include "intercode.h"
#include "type-symbol.h"
#include<stdio.h>

// add elements from head in all lists

//-------- OpList ---------//

struct OpListNode_def{
    Operand val_;
    struct OpListNode_def *next;
    struct OpListNode_def *prev;
};
typedef struct OpListNode_def OpListNode;

struct OpList_def{
    struct OpListNode_def *head_;
    struct OpListNode_def *tail_;
};
typedef struct OpList_def OpList;

OpList* OpListInit();
void OpListInsert(OpList* self, Operand val);

//-------- IntList ---------//

struct IntListNode_def{
    int val_;
    struct IntListNode_def *next;
    struct IntListNode_def *prev;
};
typedef struct IntListNode_def IntListNode;

struct IntList_def{
    struct IntListNode_def *head_;
    struct IntListNode_def *tail_;
};
typedef struct IntList_def IntList;

IntList* IntListInit();
void IntListInsert(IntList* self, int val);

#endif