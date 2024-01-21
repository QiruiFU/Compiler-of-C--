#include "translate.h"

#define SAVE_OP Operand old_op1=op1;Operand old_op2=op2;Operand old_op3=op3;char *old_rel=rel;
#define RELEASE_OP op1=old_op1;op2=old_op2;op3=old_op3;rel=old_rel;
#define TRANS_IR(kind) AddInterCode(ir_list, GenCode(kind, op1, op2, op3, rel))
#define LABLE_IR(lop) AddInterCode(ir_list, GenCode(LABEL, lop, op2, op3, rel))
#define GOTO_IR(lop) AddInterCode(ir_list, GenCode(GOTO, lop, op2, op3, rel))

extern int vari_cnt;
extern int label_cnt;

Operand NewLabel(){
    Operand new;
    new.kind = OP_LABEL;
    new.u.no_label = ++label_cnt;
    return new;
}

Operand NewTemp(){
    Operand new;
    new.kind = OP_TEMP;
    new.u.no_vari = ++vari_cnt;
    return new;
}

Operand GetVari(int x){
    Operand new;
    new.kind = OP_VARI;
    new.u.no_vari = x;
    return new;
}

Operand NewConst(int x){
    Operand new;
    new.kind = OP_CONST;
    new.u.val_const = x;
    return new;
}

Operand op1, op2, op3;
char *rel;


void Translate(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    if(strcmp(self->node_name_, "Program")==0){
        Translate(self->first_child_, ir_list);
    }
    else if(strcmp(self->node_name_, "ExtDefList")==0){
        Translate(self->first_child_, ir_list);
        if(self->cnt_child_ == 2) Translate(kthChild(self, 2), ir_list);
    }
    else if(strcmp(self->node_name_, "ExtDef")==0){
        if(self->cnt_child_ == 2){ // ExtDef -> Specifier SEMI;
            return;
        }
        else{
            assert(self->cnt_child_ == 3);
            if(strcmp(kthChild(self, 2)->node_name_, "ExtDecList")==0){  // no global variables
                return;
            }
            else if(strcmp(kthChild(self, 2)->node_name_, "FunDec")==0){
                TransFunDec(kthChild(self, 2), ir_list);
                TransCompSt(kthChild(self, 3), ir_list);
            }
        }
    }
    RELEASE_OP
}

#define WANT_VAL 1
#define WANT_ADD 2
void TransExp(TreeNode* self, Operand opde, InterCodeList *ir_list, int request){ 
    SAVE_OP
    assert(strcmp(self->node_name_, "Exp")==0);

    if(self->cnt_child_ == 1){
        if(strcmp(self->first_child_->node_name_, "INT")==0){
            op1 = opde;
            op2 = NewConst(self->first_child_->compos_.val_int_);
            TRANS_IR(ASSIGN_VAL2VAL);
        }
        else if(strcmp(self->first_child_->node_name_, "ID")==0){
            op1 = opde;
            op2 = GetVari(self->first_child_->inter_no_);
            TRANS_IR(ASSIGN_VAL2VAL);
        }
    }

    else if(self->cnt_child_ == 2){
        if(strcmp(self->first_child_->node_name_, "MINUS")==0){
            TransExp(kthChild(self, 2), op3, ir_list, WANT_VAL);
            op1 = opde;
            op2 = NewConst(0);
            TRANS_IR(SUB);
        }
        else if(strcmp(self->first_child_->node_name_, "NOT")==0){
            // TODO: what?
            op1 = opde;
            op2 = NewConst(0);
            TRANS_IR(ASSIGN_VAL2VAL);
            op1 = NewLabel();
            op2 = NewLabel();
            TransCond(self, op1, op2, ir_list);
            LABLE_IR(op1);
            op1 = opde;
            Operand temp = op2;
            op2 = NewConst(1);
            TRANS_IR(ASSIGN_VAL2VAL);
            LABLE_IR(temp);
        }
    }

    else if(self->cnt_child_ == 3){
        char *midname = kthChild(self, 2)->node_name_;

        if(strcmp(midname, "RELOP")==0 || strcmp(midname, "AND")==0 || strcmp(midname, "OR")==0){
            op1 = opde;
            op2 = NewConst(0);
            TRANS_IR(ASSIGN_VAL2VAL);

            Operand label_true = NewLabel();
            Operand label_false = NewLabel();
            TransCond(self, label_true, label_false, ir_list);

            LABLE_IR(label_true);

            op1 = opde;
            op2 = NewConst(1);
            TRANS_IR(ASSIGN_VAL2VAL);

            LABLE_IR(label_false);
        }

        else if(strcmp(midname, "ASSIGNOP")==0){
            op2 = NewTemp();
            TransExp(kthChild(self, 3), op2, ir_list, WANT_VAL);
            if(self->first_child_->cnt_child_==1 && strcmp(self->first_child_->first_child_->node_name_, "ID")==0){
                op1 = GetVari(self->first_child_->first_child_->inter_no_);
                TRANS_IR(ASSIGN_VAL2VAL);
            }
            else{
                op1 = NewTemp();
                TransExp(self->first_child_, op1, ir_list, WANT_ADD);
                TRANS_IR(ASSIGN_VAL2PNT);
            }
            /*
            else if(self->first_child_->cnt_child_==4 && strcmp(kthChild(self->first_child_, 2)->node_name_, "LB")==0){
                op1 = NewTemp();
                TransArray(self->first_child_, op1, ir_list);
                TRANS_IR(ASSIGN_VAL2PNT);
            }
            */
        }

        else if(strcmp(midname, "LP")==0){
            if(strcmp(self->first_child_->compos_.id_, "read")==0){
                op1 = opde;
                TRANS_IR(READ);
            }
            else{
                op1 = opde;
                op2.kind = OP_FUNC;
                op2.u.func_name = self->first_child_->node_name_;
                TRANS_IR(CALL);
            }
        }

        else if(strcmp(midname, "Exp")==0){
            TransExp(kthChild(self, 2), opde, ir_list, WANT_VAL);
        }

        else if(strcmp(midname, "DOT")==0){
            Operand st_addr = NewTemp();
            if(strcmp(self->first_child_->first_child_->node_name_, "ID")==0){
                op1 = st_addr;
                op2 = GetVari(self->first_child_->first_child_->inter_no_);
                TRANS_IR(ASSIGN_ADD2VAL);
            }
            else{
                TransExp(self->first_child_, st_addr, ir_list, WANT_ADD);
            }
            Field *fld = self->first_child_->type_->u.structure.field;
            int offset = 0;
            while(strcmp(fld->name, kthChild(self, 3)->compos_.id_) != 0){
                offset += TypeSize(fld->type);
                fld = fld->nxt;
            }
            op1 = opde;
            op2 = st_addr;
            op3 = NewConst(offset);
            TRANS_IR(ADD);

            if(request == WANT_VAL){
                op1 = opde;
                op2 = opde;
                TRANS_IR(ASSIGN_PNT2VAL);
            }
        }

        else{
            op1 = opde;
            op2 = NewTemp();
            op3 = NewTemp();
            TransExp(kthChild(self, 1), op2, ir_list, WANT_VAL);
            TransExp(kthChild(self, 3), op3, ir_list, WANT_VAL);
            if(strcmp(midname, "PLUS")==0) TRANS_IR(ADD);
            else if(strcmp(midname, "MINUS")==0) TRANS_IR(SUB);
            else if(strcmp(midname, "STAR")==0) TRANS_IR(MULTI);
            else if(strcmp(midname, "DIV")==0) TRANS_IR(DIV);
            else assert(0);
        }
    }

    else if(self->cnt_child_ == 4){
        if(strcmp(self->first_child_->node_name_, "ID")==0){ // call function
            OpList* arg_list = OpListInit();
            TransArgs(kthChild(self, 3), arg_list, ir_list);
            if(strcmp(self->first_child_->compos_.id_, "write")==0){
                op1 = arg_list->head_->val_;
                TRANS_IR(WRITE);
            }
            else{
                OpListNode *cur = arg_list->head_;
                while(cur != NULL){
                    op1 = cur->val_;
                    TRANS_IR(ARG);
                    cur = cur->next;
                }
                op1 = opde;
                op2.kind = OP_FUNC;
                op2.u.func_name = self->first_child_->compos_.id_;
                TRANS_IR(CALL);
            }
        }
        else if(strcmp(kthChild(self, 2)->node_name_, "LB")==0){ // visit array
            op1 = opde;
            op2 = NewTemp();
            TransArray(self, op2, ir_list);
            TRANS_IR(ASSIGN_VAL2VAL);
            if(request == WANT_VAL){
                op1 = opde;
                op2 = opde;
                TRANS_IR(ASSIGN_PNT2VAL);
            }
        }
    }
    RELEASE_OP
}

void TransStmt(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "Stmt")==0);

    if(self->cnt_child_==1 && strcmp(self->first_child_->node_name_, "CompSt")==0){
        TransCompSt(self->first_child_, ir_list);
    }
    else if(self->cnt_child_==2 && strcmp(self->first_child_->node_name_, "Exp")==0){
        op1 = NewTemp();
        TransExp(self->first_child_, op1, ir_list, WANT_VAL);
    }
    else if(self->cnt_child_==3 && strcmp(self->first_child_->node_name_, "RETURN")==0){
        op1 = NewTemp();
        TransExp(kthChild(self, 2), op1, ir_list, WANT_VAL);
        TRANS_IR(RETURN);
    }
    else if(self->cnt_child_==5 && strcmp(self->first_child_->node_name_, "IF")==0){
        Operand l1 = NewLabel();
        Operand l2 = NewLabel();
        TransCond(kthChild(self, 3), l1, l2, ir_list);
        LABLE_IR(l1);
        TransStmt(kthChild(self, 5), ir_list);
        LABLE_IR(l2);
    }
    else if(self->cnt_child_==7){
        Operand l1 = NewLabel();
        Operand l2 = NewLabel();
        Operand l3 = NewLabel();
        TransCond(kthChild(self, 3), l1, l2, ir_list);

        LABLE_IR(l1);
        TransStmt(kthChild(self, 5), ir_list);
        GOTO_IR(l3);
        LABLE_IR(l2);
        TransStmt(kthChild(self, 7), ir_list);
        LABLE_IR(l3);
    }
    else if(self->cnt_child_==5 && strcmp(self->first_child_->node_name_, "WHILE")==0){
        Operand l1 = NewLabel();
        Operand l2 = NewLabel();
        Operand l3 = NewLabel();

        LABLE_IR(l1);
        TransCond(kthChild(self, 3), l2, l3, ir_list);
        LABLE_IR(l2);
        TransStmt(kthChild(self, 5), ir_list);
        GOTO_IR(l1);
        LABLE_IR(l3);
    }
    RELEASE_OP
}

void TransCond(TreeNode* self, Operand lt, Operand lf, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "Exp")==0);

    if(self->cnt_child_ == 3){
        char *midname = kthChild(self, 2)->node_name_;
        if(strcmp(midname, "RELOP")==0){
            op1 = NewTemp();
            op2 = NewTemp();
            TransExp(kthChild(self, 1), op1, ir_list, WANT_VAL);
            TransExp(kthChild(self, 3), op2, ir_list, WANT_VAL);
            rel = kthChild(self, 2)->compos_.id_;
            op3 = lt;
            TRANS_IR(GOTO_COND);
            GOTO_IR(lf);
        }
        else if(strcmp(midname, "AND")==0){
            Operand l1 = NewLabel();
            TransCond(kthChild(self, 1), l1, lf, ir_list);
            LABLE_IR(l1);
            TransCond(kthChild(self, 3), lt, lf, ir_list);
        }
        else if(strcmp(midname, "OR")==0){
            Operand l1 = NewLabel();
            TransCond(kthChild(self, 1), lt, l1, ir_list);
            LABLE_IR(l1);
            TransCond(kthChild(self, 3), lt, lf, ir_list);
        }
    }

    else if(self->cnt_child_==2 && strcmp(self->first_child_->node_name_, "NOT")==0){
        TransCond(kthChild(self, 2), lt, lf, ir_list);
    }

    else{
        op1 = NewTemp();
        TransExp(self, op1, ir_list, WANT_VAL);
        op2 = NewConst(0);
        op3 = lt;
        rel = (char*)"!=";
        TRANS_IR(GOTO_COND);
        GOTO_IR(lf);
    }
    RELEASE_OP
}

void TransCompSt(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "CompSt")==0);
    if(self->cnt_child_==3){
        TransStmtList(kthChild(self, 2), ir_list);
    }
    RELEASE_OP
}

void TransFunDec(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "FunDec")==0);
    op1.kind = OP_FUNC;
    op1.u.func_name = self->first_child_->compos_.id_;
    TRANS_IR(FUNC);
    if(self->cnt_child_ == 4){
        TransVarList(kthChild(self, 3), ir_list);
    }
    RELEASE_OP
}

void TransDefList(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "DefList")==0);
    TransDef(self->first_child_, ir_list);
    if(self->cnt_child_ == 2) TransDefList(kthChild(self, 2), ir_list);
    RELEASE_OP
}

void TransDef(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "Def")==0);
    TransDecList(kthChild(self, 2), ir_list);
    RELEASE_OP
}

void TransDecList(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "DecList")==0);
    TransDec(self->first_child_, ir_list);
    if(self->cnt_child_==3) TransDecList(kthChild(self, 3), ir_list);
    RELEASE_OP
}

void TransDec(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "Dec")==0);
    if(self->cnt_child_ == 3){ // assign
        op2 = NewTemp();
        TransExp(kthChild(self, 3), op2, ir_list, WANT_VAL);
        op1 = GetVari(self->first_child_->first_child_->inter_no_);
        TRANS_IR(ASSIGN_VAL2VAL);
    }
    else if(self->cnt_child_ == 1){
        TransVarDec(self->first_child_, SC_DEC, 0, ir_list);
    }
    RELEASE_OP
}

void TransStmtList(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "StmtList")==0);
    if(strcmp(self->first_child_->node_name_, "Stmt")==0){
        TransStmt(self->first_child_, ir_list);
    }
    else if(strcmp(self->first_child_->node_name_, "Def")==0){
        TransDef(self->first_child_, ir_list);
    }

    if(self->cnt_child_ == 2){
        TransStmtList(kthChild(self, 2), ir_list);
    }
    RELEASE_OP
}

void TransArgs(TreeNode* self, OpList *arg_list, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "Args")==0);
    if(self->first_child_->type_->kind == ARRAY){
        // TODO : arg - array
    }
    else if(self->first_child_->type_->kind == STRUCTURE){
        // TODO : arg - struct
    }
    else{
        op1 = NewTemp();
        TransExp(self->first_child_, op1, ir_list, WANT_VAL);
    }

    OpListInsert(arg_list, op1);
    if(self->cnt_child_ == 3){
        TransArgs(kthChild(self, 3), arg_list, ir_list);
    }
    RELEASE_OP
}

void TransVarDec(TreeNode* self, int source, int size, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "VarDec")==0);
    assert(source==SC_DEC || source==SC_FUNC);

    if(source == SC_FUNC){
        if(self->cnt_child_ == 1){ // ID
            op1 = GetVari(self->first_child_->inter_no_);
            TRANS_IR(PARAM);
        }
        else{ // para is an array
            TreeNode *find_id = self;
            while(strcmp(find_id->node_name_, "ID")!=0) find_id = find_id->first_child_;
            // TODO: para is an array
        }
    }
    else{
        if(self->cnt_child_ == 1){ // Var -> ID
            if(size == 0) {
                if(self->first_child_->type_->kind == BASIC) return; // normal vari
                int struct_size = TypeSize(self->first_child_->type_);
                op1 = GetVari(self->first_child_->inter_no_);
                op2 = NewConst(struct_size);
                TRANS_IR(DEC);
            }
            else{
                // declare array
                op1 = GetVari(self->first_child_->inter_no_);
                TreeNode *get_type = self;
                while(strcmp(get_type->node_name_, "ID")!=0) get_type = get_type->first_child_;
                op2 = NewConst(size * TypeSize(get_type->type_->u.array.elem));
                TRANS_IR(DEC);
            }
        }
        else{ // VarDec -> VarDec LB INT RB
            if(size==0) size = 1;
            size *= kthChild(self, 3)->compos_.val_int_;
            TransVarDec(self->first_child_, source, size, ir_list);
        }
    }
    RELEASE_OP
}

void TransParamDec(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "ParamDec")==0);
    TransVarDec(kthChild(self, 2), SC_FUNC, 0, ir_list);
    RELEASE_OP
}

void TransVarList(TreeNode* self, InterCodeList *ir_list){
    SAVE_OP
    assert(strcmp(self->node_name_, "VarList")==0);
    TransParamDec(self->first_child_, ir_list);
    if(self->cnt_child_ == 3){
        TransVarList(kthChild(self, 3), ir_list);
    }
    RELEASE_OP
}

void TransArray(TreeNode* self, Operand opde, InterCodeList *ir_list){
    SAVE_OP
    TreeNode *cur = self;
    while(cur->cnt_child_ == 4) cur = cur->first_child_;
    Type *dim = cur->type_;
    IntList *dim_list = IntListInit();

    while(dim->kind == ARRAY){ // make dims a list
        IntListInsert(dim_list, dim->u.array.size);
        dim = dim->u.array.elem;
    }

    Operand addr = NewTemp();
    op1 = addr;
    op2 = NewConst(0);
    TRANS_IR(ASSIGN_VAL2VAL);

    IntListNode *cur_dim = dim_list->head_;
    int sub_size = 1;
    cur = self;
    while(cur_dim != NULL){
        Operand dim_size = NewTemp();
        TransExp(kthChild(cur, 3), dim_size, ir_list, WANT_VAL);
        op1 = dim_size;
        op2 = dim_size;
        op3 = NewConst(sub_size);
        TRANS_IR(MULTI);

        op1 = addr;
        op2 = addr;
        op3 = dim_size;
        TRANS_IR(ADD);

        sub_size *= cur_dim->val_;
        cur = cur->first_child_;
        cur_dim = cur_dim->next;
    }
    // assert(strcmp(cur->first_child_->node_name_, "ID")==0);
    op1 = addr;
    op2 = addr;
    op3 = NewConst(TypeSize(kthChild(cur, cur->cnt_child_)->type_->u.array.elem));
    TRANS_IR(MULTI);

    TreeNode *check_global = cur;
    while(strcmp(check_global->node_name_, "ExtDef")!=0) check_global = check_global->father_;
    check_global = kthChild(check_global, 2)->first_child_;
    assert(strcmp(check_global->node_name_, "ID")==0);

    if(strcmp(check_global->compos_.id_, "main")==0){
        Operand start_addr = NewTemp();
        if(strcmp(cur->first_child_->node_name_, "Exp")==0){
            TransExp(cur, start_addr, ir_list, WANT_ADD);
        }
        else{
            op1 = start_addr;
            op2 = GetVari(cur->first_child_->inter_no_);
            TRANS_IR(ASSIGN_ADD2VAL);
        }
        op1 = opde;
        op2 = addr;
        op3 = start_addr;
        TRANS_IR(ADD);
    }
    else{
        Operand start_addr = NewTemp();
        if(strcmp(cur->first_child_->node_name_, "Exp")==0){
            TransExp(cur, start_addr, ir_list, WANT_ADD);
        }
        else{
            op1 = start_addr;
            op2 = GetVari(cur->first_child_->inter_no_);
            TRANS_IR(ASSIGN_VAL2VAL);
        }
        op1 = opde;
        op2 = addr;
        op3 = start_addr;
        TRANS_IR(ADD);
    }
    RELEASE_OP
}
