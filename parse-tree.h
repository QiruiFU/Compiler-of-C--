#ifndef PARSE_TREE
#define PARSE_TREE

// #include "Hash.h"
#include "type-symbol.h"

struct TreeNodeDef{
    char *node_name_;
    int cnt_child_;
    int line_;
    struct TreeNodeDef* father_;
    struct TreeNodeDef* first_child_;
    struct TreeNodeDef* brother_;
    union{
        char *id_;
        int val_int_;
        float val_float_;
    } compos_;
    int anaylised_;
    Type* type_;
    int assignable_;
}; // a typical structure of tree
typedef struct TreeNodeDef TreeNode;

TreeNode* TreeNodeInit(char node_name[], int line, char text[]);
TreeNode* Fatherize(char node_name[], int line, int cnt_child, TreeNode* children[7]); // generate a father node of children nodes
TreeNode* kthChild(TreeNode* self, int k); // return kth child node
void ProcessNode(TreeNode* self);

struct ParseTreeDef{
    TreeNode* root_;
};
typedef struct ParseTreeDef ParseTree;

ParseTree* ParseTreeInit(TreeNode* root);
void PrintTree(ParseTree* self);
void ProcessTree(ParseTree* self);


// void Check(struct tree_node* root); // 语义检查
// void Insert(struct tree_node* cur);
// int ExistStruct(struct tree_node *root);

#endif