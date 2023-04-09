#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "ParseTree.h"

extern int lineno;
extern int cnt_False;
extern int yydebug;
extern void yyrestart(FILE*);
extern int yyparse();

struct tree_node* ROOT = NULL; // the root of the parse tree

int main(int argc, char** argv){
    if(argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f){
        perror(argv[1]);
        return 1;
    }

    yyrestart(f);
    // yydebug = 1;
    yyparse();

    if(cnt_False == 0){
        print_tree(ROOT, 0);
    }

    return 0;
}