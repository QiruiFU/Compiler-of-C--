#ifndef TARGETCODE
#define TARGETCODE

#include<stdio.h>
#include<assert.h>
#include "intercode.h"

void init_tar(FILE *fout);
void TgtCodeList(InterCodeList *CodeList, FILE *fout);
void TgtCode(InterCode code, FILE *fout);
void TgtAdd(InterCode code, FILE *fout);
void TgtSub(InterCode code, FILE *fout);
void TgtMulti(InterCode code, FILE *fout);
void TgtDiv(InterCode code, FILE *fout);
void TgtAssignVal2Val(InterCode code, FILE *fout);
void TgtAssignVal2Pnt(InterCode code, FILE *fout);
void TgtAssignPnt2Val(InterCode code, FILE *fout);
void TgtAssignAdd2Val(InterCode code, FILE *fout);
void TgtGoCond(InterCode code, FILE *fout);
void TgtReturn(InterCode code, FILE *fout);
void TgtRead(InterCode code, FILE *fout);
void TgtFunc(InterCode code, FILE *fout);
void TgtWrite(InterCode code, FILE *fout);
void TgtDec(InterCode code, FILE *fout);
void TgtArg(InterCode code, FILE *fout);
void TgtCall(InterCode code, FILE *fout);
void TgtParam(InterCode code, FILE *fout);

int AddrVar(Operand op); // offset of fp
void DeterAddr(Operand op, int offset);
void my_alloc(InterCodeNode *code);

struct VariList_def{
    Operand op;
    int offset;
    struct VariList_def *nxt;
};
typedef struct VariList_def VariList;

#endif