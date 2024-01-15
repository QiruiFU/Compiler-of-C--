#ifndef TRANSLATE
#define TRANSLATE

#include<stdio.h>
#include "intercode.h"
#include "parse-tree.h"
#include "type-symbol.h"
#include "list.h"

#define SC_FUNC 1
#define SC_DEC 2

void Translate(TreeNode* self, InterCodeList *ir_list);
void TransExp(TreeNode* self, Operand opde, InterCodeList *ir_list, int request);
void TransStmt(TreeNode* self, InterCodeList *ir_list);
void TransCond(TreeNode* self, Operand lt, Operand lf, InterCodeList *ir_list);
void TransCompSt(TreeNode* self, InterCodeList *ir_list);
void TransFunDec(TreeNode* self, InterCodeList *ir_list);
void TransDefList(TreeNode* self, InterCodeList *ir_list);
void TransDef(TreeNode* self, InterCodeList *ir_list);
void TransDecList(TreeNode* self, InterCodeList *ir_list);
void TransDec(TreeNode* self, InterCodeList *ir_list);
void TransStmtList(TreeNode* self, InterCodeList *ir_list);
void TransArgs(TreeNode* self, OpList *arg_list, InterCodeList *ir_list);
void TransVarDec(TreeNode* self, int source, int size, InterCodeList *ir_list);
void TransParamDec(TreeNode* self, InterCodeList *ir_list);
void TransVarList(TreeNode* self, InterCodeList *ir_list);
void TransArray(TreeNode* self, Operand opde, InterCodeList *ir_list);

#endif