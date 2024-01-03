#include "intercode.h"
#include<assert.h>

void Print_OP(Operand op, FILE *f){
    switch(op.kind){
        case OP_VARI:
            fprintf(f, "variable%d", op.u.no_vari);
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
            Print_OP(code.u.op1, f);
            fprintf(f, " :\n");
            break;
        case FUNC:
            fprintf(f, "FUNCTION ");
            Print_OP(code.u.op1, f);
            fprintf(f, " :\n");
            break;
        case ASSIGN_VAL2VAL:
            Print_OP(code.u.op1, f);
            fprintf(f, " := ");
            Print_OP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case ADD:
            Print_OP(code.u.op1, f);
            fprintf(f, " := ");
            Print_OP(code.u.op2, f);
            fprintf(f, " + ");
            Print_OP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case SUB:
            Print_OP(code.u.op1, f);
            fprintf(f, " := ");
            Print_OP(code.u.op2, f);
            fprintf(f, " - ");
            Print_OP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case MULTI:
            Print_OP(code.u.op1, f);
            fprintf(f, " := ");
            Print_OP(code.u.op2, f);
            fprintf(f, " * ");
            Print_OP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case DIV:
            Print_OP(code.u.op1, f);
            fprintf(f, " := ");
            Print_OP(code.u.op2, f);
            fprintf(f, " / ");
            Print_OP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case ASSIGN_ADD2VAL:
            Print_OP(code.u.op1, f);
            fprintf(f, " := &");
            Print_OP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case ASSIGN_PNT2VAL:
            Print_OP(code.u.op1, f);
            fprintf(f, " := *");
            Print_OP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case ASSIGN_VAL2PNT:
            fprintf(f, "*");
            Print_OP(code.u.op1, f);
            fprintf(f, " := ");
            Print_OP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case GOTO:
            fprintf(f, "GOTO ");
            Print_OP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case GOTO_COND:
            fprintf(f, "IF ");
            Print_OP(code.u.op1, f);
            fprintf(f, " %s ", code.u.rel);
            Print_OP(code.u.op2, f);
            fprintf(f, " GOTO ");
            Print_OP(code.u.op3, f);
            fprintf(f, "\n");
            break;
        case RETURN:
            fprintf(f, "RETURN ");
            Print_OP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case DEC:
            fprintf(f, "DEC ");
            Print_OP(code.u.op1, f);
            fprintf(f, " %d\n", code.u.op2.u.val_const);
            break;
        case ARG:
            fprintf(f, "ARG ");
            Print_OP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case CALL:
            Print_OP(code.u.op1, f);
            fprintf(f, " := CALL ");
            Print_OP(code.u.op2, f);
            fprintf(f, "\n");
            break;
        case PARAM:
            fprintf(f, "PARAM ");
            Print_OP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case READ:
            fprintf(f, "READ ");
            Print_OP(code.u.op1, f);
            fprintf(f, "\n");
            break;
        case WRITE:
            fprintf(f, "WRITE ");
            Print_OP(code.u.op1, f);
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

void PrintList(InterCodeList* self, FILE *f){
    InterCodeNode* cur = self->head_;
    while(cur!=NULL){
        Print_Code(cur->node_, f);
        cur = cur->next;
    }
    return;
}