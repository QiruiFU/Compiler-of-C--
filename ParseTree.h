#ifndef PARSETREE
#define PARSETREE

#include "Hash.h"
#include "type_func.h"

typedef union{
    char id[32];
    int val_int;
    float val_float;
} token;

struct tree_node{
    char name[32];
    int cnt_child;
    int first_line;
    struct tree_node* father;
    struct tree_node* first_child;
    struct tree_node* brother;
    token compos;
    Type* type;
    int assignable;
    int anaylised;
}; // a typical structure of a tree, brother saves the address of brother


struct tree_node* fatherize(char* name, int line, int cnt_child, struct tree_node* children[20]);
void print_tree(struct tree_node* root, int depth);
void Check(struct tree_node* root); // 语义检查
void Insert(struct tree_node* cur);
struct tree_node* child_of_no(int x, struct tree_node* root);
int ExistStruct(struct tree_node *root);

#endif