#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
// #include "intercode.h"
#include "parse-tree.h"
// #include "Hash.h"
// #include "translate.h"
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

// InterCodeNode *CodeList = NULL;
ParseTree *parse_tree; // parse tree
// Type iint; // global define of type int

Stack* page_stack;

// void initiate(){
//     iint.kind = BASEE;
//     iint.type.base = INTT;

//     Symbol sym_read;
//     sym_read.kind = FUNCTIONN;
//     strcpy(sym_read.name, "read");
//     sym_read.prop.sym_func = (Func*)malloc(sizeof(Func));
//     sym_read.prop.sym_func->Argc_cnt = 0;
//     sym_read.prop.sym_func->Argv = NULL;
//     sym_read.prop.sym_func->retn = &iint;

//     Symbol sym_write;
//     sym_write.kind = FUNCTIONN;
//     strcpy(sym_write.name, "write");
//     sym_write.prop.sym_func = (Func*)malloc(sizeof(Func));
//     sym_write.prop.sym_func->Argv = (Field*)malloc(sizeof(Field));
//     sym_write.prop.sym_func->Argc_cnt = 1;
//     sym_write.prop.sym_func->Argv->type_field = &iint;
//     sym_write.prop.sym_func->Argv->nxt = NULL;
//     sym_read.prop.sym_func->retn = NULL;

//     Hash_Add(Stack_top(page_stack), sym_read);
//     Hash_Add(Stack_top(page_stack), sym_write);

//     CodeList = (InterCodeNode*)malloc(sizeof(InterCodeNode));
//     CodeList->next = NULL;
// }

int main(int argc, char** argv){
    FILE* fin = fopen(argv[1], "r");
    // FILE* fout = fopen(argv[2], "w");

    if (!fin){
        perror(argv[1]);
        return 1;
    }
    // if (!fout){
    //     perror(argv[2]);
    //     return 1;
    // }

    page_stack = StackInit();

    yyrestart(fin);
    // yydebug = 1;
    yyparse();

    if(cnt_error == 0){
        ProcessTree(parse_tree);
        // PrintTree(parse_tree);
        // initiate();
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
    // fclose(fout);

    return 0;
}
