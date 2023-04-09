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

    struct tree_node* cur = new_p->first_child;
    assert(cur->brother == NULL);
    for(int i=1; i<cnt_child; i++){
        if(children[i]==NULL) continue;
        cur->brother = children[i];
        cur = cur->brother;
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
