// #ifndef TRANSLATE
// #define TRANSLATE

// #include<stdio.h>
// #include "intercode.h"
// #include "ParseTree.h"
// #include "Hash.h"
// #include "type_func.h"

// #define SC_FUNC 1
// #define SC_DEC 2

// struct listnode_def{
//     Operand para;
//     struct listnode_def *next;
// };
// typedef struct listnode_def listnode;

// struct intlist_def{
//     int val;
//     struct intlist_def *next;
// };
// typedef struct intlist_def intlist;

// void Translate(struct tree_node *root);
// void Trans_Exp(struct tree_node *root, Operand opde);
// void Trans_Stmt(struct tree_node* root);
// void Trans_Cond(struct tree_node *root, Operand lt, Operand lf);
// void Translate(struct tree_node *root);
// void Trans_CompSt(struct tree_node *root);
// void Trans_FunDec(struct tree_node *root);
// void Trans_DefList(struct tree_node *root);
// void Trans_Def(struct tree_node *root);
// void Trans_DecList(struct tree_node *root);
// void Trans_Dec(struct tree_node *root);
// void Trans_StmtList(struct tree_node *root);
// void Trans_Args(struct tree_node *root, listnode **arg_list);
// void Trans_VarDec(struct tree_node *root, int source, int size);
// void Trans_ParamDec(struct tree_node *root);
// void Trans_VarList(struct tree_node *root);
// void Trans_Array(struct tree_node *root, Operand opde);

// #endif