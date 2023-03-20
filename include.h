#ifndef INCLUDE
#define INCLUDE

typedef union{
    char id[32];
    int val_int;
    float val_float;
} token;

struct tree_node{
    char name[32];
    int first_line;
    struct tree_node* father;
    struct tree_node* first_child;
    struct tree_node* brother;
    token compos;
}; // a typical structure of a tree, brother saves the address of brother

#endif