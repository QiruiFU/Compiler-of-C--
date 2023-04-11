#include "ParseTree.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

struct tree_node* fatherize(char* name, int line, int cnt_child, struct tree_node* children[20]){
    // build a new node, which is the father of given nodes
    assert(cnt_child>=1);
    struct tree_node* new_p = (struct tree_node*)malloc(sizeof(struct tree_node));
    strcpy(new_p->name, name);
    new_p->first_line = line;
    new_p->father = NULL;
    new_p->brother = NULL;
    new_p->compos.val_int = 0;
    new_p->first_child = children[0];
    new_p->cnt_child = 1;

    struct tree_node* cur = new_p->first_child;
    cur->father = new_p;
    assert(cur->brother == NULL);
    for(int i=1; i<cnt_child; i++){
        if(children[i]==NULL) continue;
        cur->brother = children[i];
        cur = cur->brother;
        cur->father = new_p;
        new_p->cnt_child++;
        assert(cur->brother == NULL);
    }

    return new_p;
}

void print_tree(struct tree_node* root, int depth){
    for(int i=0; i<depth*2; i++){
        printf(" ");
    }

    if(root->first_child==NULL){
        printf("%s", root->name);
        if(strcmp(root->name, "ID")==0) printf(": %s\n",root->compos.id);
        else if(strcmp(root->name, "INT")==0) printf(": %d\n",root->compos.val_int);
        else if(strcmp(root->name, "FLOAT")==0) printf(": %f\n",root->compos.val_float);
        else if(strcmp(root->name, "TYPE")==0) printf(": %s\n",root->compos.id);
        else printf("\n");
    }
    else{
        printf("%s (%d)\n", root->name, root->first_line);
    }
    
    struct tree_node* cur = root->first_child;
    while(cur!=NULL){
        print_tree(cur, depth+1);
        cur = cur->brother;
    }
}


struct tree_node* child_of_no(int x, struct tree_node* root){
    assert(x<=root->cnt_child && x>=1);
    struct tree_node* cur = root->first_child;
    int now = 1;
    while(now!=x){
        cur = cur->brother;
        now++;
    }
    return cur;
}

void Insert(struct tree_node* cur){
    if(strcmp(cur->name, "ID")!=0) return;

    Symbol sym;
    strcpy(sym.name, cur->compos.id);

    if(strcmp(cur->father->name, "OptTag")==0){
        sym.kind = STRUCTT;

        // TODO: declare a struct 
        if(!Hash_Find(&Hash_table, sym)){
            Hash_Add(&Hash_table, sym);
        }
    }
    else if(strcmp(cur->father->name, "FunDec")==0){
        sym.kind = FUNCTIONN;
        
        // TODO: declare a function
        if(!Hash_Find(&Hash_table, sym)){
            Hash_Add(&Hash_table, sym);
        }
    }
    else if(strcmp(cur->father->name, "VarDec")==0){
        sym.kind = VARIABLEE;

        //TODO: declare a variable
        if(!Hash_Find(&Hash_table, sym)){
            Hash_Add(&Hash_table, sym);
        }
    }
}

void Check_1(struct tree_node* cur){
    if(strcmp(cur->name, "ID")!=0) return;

    Symbol sym;
    sym.kind = VARIABLEE;
    strcpy(sym.name, cur->compos.id);

    if(strcmp(cur->father->name, "Exp")==0 && cur->father->cnt_child==1){
        if(!Hash_Find(&Hash_table, sym)){
            printf("Error type 1 at Line %d: undeclared variable %s\n", cur->first_line, cur->compos.id);
            return;
        }
    }
}

void Check_2(struct tree_node* cur){
    if(strcmp(cur->name, "ID")!=0) return;

    Symbol sym;
    sym.kind = FUNCTIONN;
    strcpy(sym.name, cur->compos.id);

    if(strcmp(cur->father->name, "Exp")==0 && cur->brother!=NULL && strcmp(cur->brother->name, "LP")==0){
        if(!Hash_Find(&Hash_table, sym)){
            printf("Error type 2 at Line %d: undeclared function %s\n", cur->first_line, cur->compos.id);
            return;
        }
    }
}

void Check_3(struct tree_node* cur){
    if(strcmp(cur->name, "ID")!=0) return;

    Symbol sym;
    sym.kind = VARIABLEE;
    strcpy(sym.name, cur->compos.id);

    if(strcmp(cur->father->name, "VarDec")==0){
        int res = Hash_Find(&Hash_table, sym);
        if(res==1 || res==3){
            printf("Error type 3 at Line %d: varialbe %s has been declared\n", cur->first_line, cur->compos.id);
            return;
        }
    }
}

void Check_4(struct tree_node* cur){
    if(strcmp(cur->name, "ID")!=0) return;

    Symbol sym;
    sym.kind = FUNCTIONN;
    strcpy(sym.name, cur->compos.id);

    if(strcmp(cur->father->name, "FunDec")==0){
        int res = Hash_Find(&Hash_table, sym);
        if(res==4){
            printf("Error type 4 at Line %d: function %s has been declared\n", cur->first_line, cur->compos.id);
            return;
        }
    }
}

void analyse(struct tree_node* root){
    if(root == NULL) return;

    Check_1(root);
    Check_2(root);
    Check_3(root);
    Check_4(root);
    Insert(root);

    struct tree_node* cur = root->first_child;

    while(cur!=NULL){
        analyse(cur);
        cur = cur->brother;
    }
}
