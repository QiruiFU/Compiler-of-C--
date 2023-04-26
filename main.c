#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "ParseTree.h"

// extern int lineno;
// extern int cnt_False;
// extern int yydebug;
// extern void yyrestart(FILE*);
// extern int yyparse();

struct tree_node* ROOT = NULL; // the root of the parse tree

// int main(int argc, char** argv){
//     if(argc <= 1) return 1;
//     FILE* f = fopen(argv[1], "r");
//     if (!f){
//         perror(argv[1]);
//         return 1;
//     }

//     yyrestart(f);
//     // yydebug = 1;
//     yyparse();

//     if(cnt_False == 0){
//         // print_tree(ROOT, 0);
//         // GetType(ROOT);
//         // printf("Get Type finished\n");
//         DFS(ROOT);
//     }

//     return 0;
// }


#include "intercode.h"

InterCodeNode *CodeList;

int main(int argc, char** argv){
    FILE* fout = fopen(argv[1], "w");
    if(!fout){
        perror(argv[1]);
        return 1;
    }

    CodeList = (InterCodeNode*)malloc(sizeof(InterCodeNode));
    CodeList->next = NULL;

    Operand op1, op2, op3;
    int size;
    char rel[3]=">";


    op1.kind = OP_FUNC;
    strcpy(op1.u.name_func, "main");
    Add_InterCode(CodeList, Gen_Code(FUNC, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 1;
    Add_InterCode(CodeList, Gen_Code(READ, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 10;
    op2.kind = OP_VARI;
    op2.u.no_vari = 1;
    Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 2;
    op2.kind = OP_CONST;
    op2.u.val_const = 0;
    Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 10;
    op2.kind = OP_VARI;
    op2.u.no_vari = 2;
    op3.kind = OP_LABEL;
    op3.u.no_label = 1;
    strcpy(rel, ">");
    Add_InterCode(CodeList, Gen_Code(GOTO_COND, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 2;
    Add_InterCode(CodeList, Gen_Code(GOTO, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 1;
    Add_InterCode(CodeList, Gen_Code(LABEL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 3;
    op2.kind = OP_CONST;
    op2.u.val_const = 1;
    Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 3;
    Add_InterCode(CodeList, Gen_Code(WRITE, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 3;
    Add_InterCode(CodeList, Gen_Code(GOTO, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 2;
    Add_InterCode(CodeList, Gen_Code(LABEL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 4;
    op2.kind = OP_CONST;
    op2.u.val_const = 0;
    Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 10;
    op2.kind = OP_VARI;
    op2.u.no_vari = 4;
    op3.kind = OP_LABEL;
    op3.u.no_label = 4;
    strcpy(rel, "<");
    Add_InterCode(CodeList, Gen_Code(GOTO_COND, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 5;
    Add_InterCode(CodeList, Gen_Code(GOTO, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 4;
    Add_InterCode(CodeList, Gen_Code(LABEL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 5;
    op2.kind = OP_CONST;
    op2.u.val_const = 1;
    Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 6;
    op2.kind = OP_CONST;
    op2.u.no_vari = 0;
    op3.kind = OP_VARI;
    op3.u.no_label = 5;
    Add_InterCode(CodeList, Gen_Code(SUB, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 6;
    Add_InterCode(CodeList, Gen_Code(WRITE, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 6;
    Add_InterCode(CodeList, Gen_Code(GOTO, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 5;
    Add_InterCode(CodeList, Gen_Code(LABEL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 7;
    op2.kind = OP_CONST;
    op2.u.val_const = 0;
    Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 7;
    Add_InterCode(CodeList, Gen_Code(WRITE, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 6;
    Add_InterCode(CodeList, Gen_Code(LABEL, op1, op2, op3, size, rel));

    op1.kind = OP_LABEL;
    op1.u.no_label = 3;
    Add_InterCode(CodeList, Gen_Code(LABEL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 8;
    op2.kind = OP_CONST;
    op2.u.val_const = 0;
    Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, op1, op2, op3, size, rel));

    op1.kind = OP_VARI;
    op1.u.no_vari = 8;
    Add_InterCode(CodeList, Gen_Code(RETURN, op1, op2, op3, size, rel));

    Print_List(CodeList, fout);
    fclose(fout);
    return 0;
}