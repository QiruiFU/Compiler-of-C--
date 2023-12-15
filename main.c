#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "intercode.h"
#include "ParseTree.h"
#include "Hash.h"
#include "translate.h"
#include "Stack.h"
#include "type_func.h"
#include "targetcode.h"

extern int lineno;
extern int cnt_False;
extern int yydebug;
extern void yyrestart(FILE*);
extern int yyparse();

int vari_cnt = 0;
int func_cnt = 0;
int array_cnt = 0;
int label_cnt = 0;
int add_cnt = 0;

InterCodeNode *CodeList = NULL;
struct tree_node* ROOT = NULL; // the root of the parse tree
Type iint; // global define of type int

Stack* page_stack;

void initiate(){
    iint.kind = BASEE;
    iint.type.base = INTT;

    Symbol sym_read;
    sym_read.kind = FUNCTIONN;
    strcpy(sym_read.name, "read");
    sym_read.prop.sym_func = (Func*)malloc(sizeof(Func));
    sym_read.prop.sym_func->Argc_cnt = 0;
    sym_read.prop.sym_func->Argv = NULL;
    sym_read.prop.sym_func->retn = &iint;

    Symbol sym_write;
    sym_write.kind = FUNCTIONN;
    strcpy(sym_write.name, "write");
    sym_write.prop.sym_func = (Func*)malloc(sizeof(Func));
    sym_write.prop.sym_func->Argv = (Field*)malloc(sizeof(Field));
    sym_write.prop.sym_func->Argc_cnt = 1;
    sym_write.prop.sym_func->Argv->type_field = &iint;
    sym_write.prop.sym_func->Argv->nxt = NULL;
    sym_read.prop.sym_func->retn = NULL;

    Hash_Add(Stack_top(page_stack), sym_read);
    Hash_Add(Stack_top(page_stack), sym_write);

    CodeList = (InterCodeNode*)malloc(sizeof(InterCodeNode));
    CodeList->next = NULL;
}

int main(int argc, char** argv){
    FILE* fin = fopen(argv[1], "r");
    if (!fin){
        perror(argv[1]);
        return 1;
    }

    page_stack = Stack_init();

    yyrestart(fin);
    // yydebug = 1;
    yyparse();

    if(cnt_False == 0){
        // print_tree(ROOT, 0);
        initiate();
        int exist = ExistStruct(ROOT);
        if(exist==1){
            fclose(fin);
            printf("Cannot translate: Code contains variables or parameters of structure type.\n");
            return 0;
        }
        FILE* fout = fopen(argv[2], "w");
        if(!fout){
            perror(argv[2]);
            return 1;
        }
        Check(ROOT);
        Translate(ROOT);
        
        // Print_List(CodeList, fout);
        // printf("here\n");
        TgtCodeList(CodeList, fout);
        fclose(fin);
        fclose(fout);
    }

    return 0;
}
