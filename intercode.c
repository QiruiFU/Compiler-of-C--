#include "intercode.h"
#include<assert.h>

void PrintOP(Operand op, FILE *f){
    switch(op.kind){
        case OP_VARI:
            fprintf(f, "v%d", op.u.no_vari);
            break;
        case OP_TEMP:
            fprintf(f, "t%d", op.u.no_vari);
            break;
        case OP_FUNC:
            fprintf(f, "%s", op.u.func_name);
            break;
        case OP_LABEL:
            fprintf(f, "label%d", op.u.no_label);
            break;
        case OP_CONST:
            fprintf(f, "#%d", op.u.val_const);
            break;
        default:
            assert(0);
    }
}

void PrintCode(InterCode code, FILE* f){
    switch(code.kind){
        case LABEL:
            fprintf(f, "LABEL ");
            PrintOP(code.u.op1, f);
            fprintf(f, " :\n");
            break;
        case FUNC:
            fprintf(f, "FUNCTION ");
            PrintOP(code.u.op1, f);
            fprintf(f, " :\n");
            break;
        case ASSIGN_VAL2VAL:
            PrintOP(code.u.op1, f);
            fprintf(f, " := ");
            PrintOP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case ADD:
            PrintOP(code.u.op1, f);
            fprintf(f, " := ");
            PrintOP(code.u.op2, f);
            fprintf(f, " + ");
            PrintOP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case SUB:
            PrintOP(code.u.op1, f);
            fprintf(f, " := ");
            PrintOP(code.u.op2, f);
            fprintf(f, " - ");
            PrintOP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case MULTI:
            PrintOP(code.u.op1, f);
            fprintf(f, " := ");
            PrintOP(code.u.op2, f);
            fprintf(f, " * ");
            PrintOP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case DIV:
            PrintOP(code.u.op1, f);
            fprintf(f, " := ");
            PrintOP(code.u.op2, f);
            fprintf(f, " / ");
            PrintOP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case ASSIGN_ADD2VAL:
            PrintOP(code.u.op1, f);
            fprintf(f, " := &");
            PrintOP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case ASSIGN_PNT2VAL:
            PrintOP(code.u.op1, f);
            fprintf(f, " := *");
            PrintOP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case ASSIGN_VAL2PNT:
            fprintf(f, "*");
            PrintOP(code.u.op1, f);
            fprintf(f, " := ");
            PrintOP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case GOTO:
            fprintf(f, "GOTO ");
            PrintOP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case GOTO_COND:
            fprintf(f, "IF ");
            PrintOP(code.u.op1, f);
            fprintf(f, " %s ", code.u.rel);
            PrintOP(code.u.op2, f);
            fprintf(f, " GOTO ");
            PrintOP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case RETURN:
            fprintf(f, "RETURN ");
            PrintOP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case DEC:
            fprintf(f, "DEC ");
            PrintOP(code.u.op1, f);
            fprintf(f, " %d\n", code.u.op2.u.val_const);
            break;
        case ARG:
            fprintf(f, "ARG ");
            PrintOP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case CALL:
            PrintOP(code.u.op1, f);
            fprintf(f, " := CALL ");
            PrintOP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case PARAM:
            fprintf(f, "PARAM ");
            PrintOP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case READ:
            fprintf(f, "READ ");
            PrintOP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case WRITE:
            fprintf(f, "WRITE ");
            PrintOP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        default:
            assert(0);
    }
}

InterCode GenCode(int kind, Operand op1, Operand op2, Operand op3, char rel[]){
    InterCode res;
    res.kind = kind;
    res.u.op1 = op1;
    res.u.op2 = op2;
    res.u.op3 = op3;
    if(rel != NULL)
        strcpy(res.u.rel, rel);
    return res;
}

InterCodeList* CodeListInit(){
    InterCodeList* self = (InterCodeList*)malloc(sizeof(InterCodeList));
    self->head_ = NULL;
    self->tail_ = NULL;
    return self;
}

void AddInterCode(InterCodeList* self, InterCode code){
    InterCodeNode *cur_tail = self->tail_;
    InterCodeNode *p = (InterCodeNode*)malloc(sizeof(InterCodeNode));
    p->node_ = code;
    p->next = NULL;

    if(cur_tail==NULL){
        p->prev = NULL;
        self->head_ = self->tail_ = p;
    }
    else{
        p->prev = cur_tail;
        cur_tail->next = p;
        self->tail_ = p;
    }
}

void PrintCodeList(InterCodeList* self, FILE *f){
    InterCodeNode* cur = self->head_;
    while(cur!=NULL){
        PrintCode(cur->node_, f);
        cur = cur->next;
    }
    return;
}