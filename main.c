#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "intercode.h"
#include "parse-tree.h"
// #include "Hash.h"
#include "translate.h"
#include "stack.h"
// #include "type_func.h"
// #include "targetcode.h"

extern int lineno;
extern int cnt_error;
extern int yydebug;
extern void yyrestart(FILE*);
extern int yyparse();

int vari_cnt = 0;
int label_cnt = 0;

ParseTree *parse_tree; // parse tree
Stack *page_stack; // symbol table
InterCodeList *inter_code_list;
Type iint; // global define of type int

void initiate(){
    iint.kind = BASIC;
    iint.u.basic = INTT;

    Symbol sym_read;
    sym_read.kind = FUNCTION;
    sym_read.name = (char*)"read";
    sym_read.u.sym_func = (Function*)malloc(sizeof(Function));
    sym_read.u.sym_func->Argc_cnt = 0;
    sym_read.u.sym_func->Argv = NULL;
    sym_read.u.sym_func->retn = &iint;

    Symbol sym_write;
    sym_write.kind = FUNCTION;
    sym_write.name = (char*)"write";
    sym_write.u.sym_func = (Function*)malloc(sizeof(Function));
    sym_write.u.sym_func->Argv = (Field*)malloc(sizeof(Field));
    sym_write.u.sym_func->Argv->type = &iint;
    sym_write.u.sym_func->Argv->nxt = NULL;
    sym_write.u.sym_func->Argc_cnt = 1;
    sym_write.u.sym_func->retn = NULL;

    HashAdd(StackTop(page_stack), sym_read);
    HashAdd(StackTop(page_stack), sym_write);
}

int main(int argc, char** argv){
    FILE* fin = fopen(argv[1], "r");
    FILE* fout = fopen(argv[2], "w");

    if (!fin){
        perror(argv[1]);
        return 1;
    }
    if (!fout){
        perror(argv[2]);
        return 1;
    }

    yyrestart(fin);
    // yydebug = 1;
    yyparse();

    if(cnt_error == 0){
        page_stack = StackInit();
        inter_code_list = CodeListInit();
        initiate();
        ProcessTree(parse_tree);
        PrintTree(parse_tree);
        Translate(parse_tree->root_, inter_code_list);
        PrintCodeList(inter_code_list, fout);
        // int exist = ExistStruct(ROOT);
        // if(exist==1){
        //     printf("Cannot translate: Code contains variables or parameters of structure type.\n");
        //     fclose(fin);
        //     fclose(fout);
        //     return 0;
        // }

        // Translate(ROOT);

        // FILE* ir = fopen("ir.ir", "w");
        // Print_List(CodeList, ir);
        // fclose(ir);

        // TgtCodeList(CodeList, fout);

    }
    fclose(fin);
    fclose(fout);

    return 0;
}
