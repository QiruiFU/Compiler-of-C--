#ifndef INTERCODE
#define INTERCODE

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

struct Operand_def{
    enum{
        OP_VARI,
        OP_TEMP,
        OP_LABEL,
        OP_FUNC,
        OP_CONST
    } kind;

    union{
        int no_vari;
        int no_label;
        char *func_name;
        int val_const;
    } u;
};
typedef struct Operand_def Operand;

struct InterCode_def{
    enum{
        LABEL,
        FUNC,
        ASSIGN_VAL2VAL,
        ADD,
        SUB,
        MULTI,
        DIV,
        ASSIGN_ADD2VAL,
        ASSIGN_PNT2VAL,
        ASSIGN_VAL2PNT,
        GOTO,
        GOTO_COND,
        RETURN,
        DEC,
        ARG,
        CALL,
        PARAM,
        READ,
        WRITE
    } kind;

    struct{
        Operand op1, op2, op3;
        char rel[3]; 
    } u;
};
typedef struct InterCode_def InterCode;

void PrintCode(InterCode code, FILE* f);
InterCode GenCode(int kind, Operand op1, Operand p2, Operand op3, char rel[]);

////------------////

struct InterCodeNode_def{
    InterCode node_;
    struct InterCodeNode_def *prev, *next; 
};
typedef struct InterCodeNode_def InterCodeNode;

struct InterCodeList_def{
    InterCodeNode* head_;
    InterCodeNode* tail_;
};
typedef struct InterCodeList_def InterCodeList;

InterCodeList* CodeListInit();
void AddInterCode(InterCodeList* self, InterCode code);
void PrintCodeList(InterCodeList* self, FILE* f);

#endif