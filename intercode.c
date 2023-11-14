#include "intercode.h"
#include<assert.h>

void Add_InterCode(InterCodeNode* head, InterCode code){
    InterCodeNode *cur = head;
    InterCodeNode *p = (InterCodeNode*)malloc(sizeof(InterCodeNode));
    p->node = code;
    p->next = NULL;

    while(cur->next!=NULL) cur = cur->next;

    cur->next = p;
}


void Print_OP(Operand op, FILE *f){
    switch(op.kind){
        case OP_VARI:
            fprintf(f, "variable%d", op.u.no_vari);
            break;
        case OP_FUNC:
            fprintf(f, "%s", op.u.name_func);
            break;
        case OP_ARRAY:
            fprintf(f, "&array%d", op.u.no_array);
            break;
        case OP_LABEL:
            fprintf(f, "label%d", op.u.no_label);
            break;
        case OP_CONST:
            fprintf(f, "#%d", op.u.val_const);
            break;
        case OP_ADD:
            fprintf(f, "addr%d", op.u.no_add);
            break;
        default:
            assert(0);
    }
}

void Print_Code(InterCode code, FILE* f){
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
            fprintf(f, " %d\n", code.u.size);
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

void Print_List(InterCodeNode* head, FILE *f){
    InterCodeNode* cur = head->next;
    while(cur!=NULL){
        Print_Code(cur->node, f);
        cur = cur->next;
    }
    return;
}

InterCode Gen_Code(int kind, Operand op1, Operand op2, Operand op3, int size, char *rel){
    InterCode res;
    res.kind = kind;
    res.u.op1 = op1;
    res.u.op2 = op2;
    res.u.op3 = op3;
    res.u.size = size;
    strcpy(res.u.rel, rel);
    return res;
}
