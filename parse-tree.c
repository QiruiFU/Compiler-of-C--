#include "parse-tree.h"
#include "stack.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

extern Stack* page_stack;

void InsertSymbol(TreeNode* self){
    TreeNode* cur = self;
    if(strcmp(cur->node_name_, "ID")!=0) return;

    Symbol sym;
    // sym.name = (char*)malloc(strlen(cur->compos_.id_)+1);
    // strcpy(sym.name, cur->compos_.id_);
    sym.name = cur->compos_.id_;

    if(strcmp(cur->father_->node_name_, "OptTag")==0){
        sym.kind = STRUCTT;
        // declare a struct 
        TreeNode* DefLst = cur->father_->brother_->brother_;
        DefLst->type_ = (Type*)malloc(sizeof(Type));
        assert(strcmp(DefLst->node_name_, "DefList")==0);
        sym.u.sym_type = DefLst->type_;

        HashTableNode* node = StackFind(page_stack, sym);
        if(node==NULL || node->symbol_.kind==FUNCTION){
            HashAdd(StackTop(page_stack), sym);
        }
        else{
            printf("Error type 16 at Line %d: name of structure %s is repeated\n", cur->line_, cur->compos_.id_);
        }
    }
    else if(strcmp(cur->father_->node_name_, "FunDec")==0){
        cur = cur->father_;
        sym.kind = FUNCTION;
        
        // declare a function
        sym.u.sym_func = (Function*)malloc(sizeof(Function));
        sym.u.sym_func->retn = cur->father_->first_child_->type_;
        sym.u.sym_func->Argc_cnt = 0;
        sym.u.sym_func->Argv = NULL;
        if(cur->cnt_child_==4){ // ID LP VarList RP
            TreeNode* para = kthChild(cur, 3);
            Field* tail = NULL;

            while(1){
                assert(strcmp(para->node_name_, "VarList")==0);
                sym.u.sym_func->Argc_cnt++;
                
                Field* new_para = (Field*)malloc(sizeof(Field));
                new_para->type = kthChild(para->first_child_, 2)->type_;
                new_para->nxt = NULL;

                if(tail==NULL){
                    tail = new_para;
                    sym.u.sym_func->Argv = new_para;
                }
                else{
                    tail->nxt = new_para;
                    tail = tail->nxt;
                }


                if(para->cnt_child_ == 1) break;
                else para = kthChild(para, 3);
            }
        };

        HashTableNode* node = StackFind(page_stack, sym);
        if(node==NULL || node->symbol_.kind!=FUNCTION){
            HashAdd(StackTop(page_stack), sym);
        }
        else{
            printf("Error type 4 at Line %d: function %s has been declared\n", self->line_, self->compos_.id_);
        }
    }
    else if(strcmp(cur->father_->node_name_, "VarDec")==0){
        TreeNode* check = cur;
        int judge = 0;
        while(check!=NULL && strcmp(check->node_name_, "StructSpecifier")!=0) check = check->father_;

        if(check!=NULL){ // a field of a struct
            judge = 1;
            // Symbol sym;
            // sym.kind = VARIABLEE;
            // strcpy(sym.name, cur->compos_.id_);

            // HashTableNode* node = Hash_Find(&Hash_table, sym);
            // if(node==NULL || node->symbol.kind==FUNCTIONN){
            //     Hash_Add(&Hash_table, sym);
            // }
            // else{
            //     printf("Error type 15 at Line %d: field %s has been declared\n", cur->line_, cur->compos_.id_);
            // }

            // if(cur->father_->brother_!=NULL && strcmp(cur->father_->brother_->name, "ASSIGNOP")==0){
            //     printf("Error type 15 at Line %d: initialize the field %s in declaration\n", cur->line_, cur->compos_.id_);
            // }
            // return;
        }

        sym.kind = VARIABLE;
        // declare a variable
        if(cur->father_->brother_==NULL || strcmp(cur->father_->brother_->node_name_, "LB")!=0){
            while(strcmp(cur->father_->first_child_->node_name_, "Specifier")!=0) cur = cur->father_;
            sym.u.sym_type = cur->father_->first_child_->type_;
        }
        else{
            cur = cur->father_;
            sym.u.sym_type = (Type*)malloc(sizeof(Type));
            sym.u.sym_type->kind = ARRAY;

            Type* tail = NULL;

            while(strcmp(cur->father_->node_name_, "VarDec")==0){
                Type* Dem = (Type*)malloc(sizeof(Type));
                Dem->kind = ARRAY;
                Dem->u.array.size = kthChild(cur->father_, 3)->compos_.val_int_;
                Dem->u.array.elem = NULL;
                if(tail == NULL){
                    tail = Dem;
                    sym.u.sym_type = Dem;
                }
                else{
                    tail->u.array.elem = Dem;
                    tail = Dem;
                }
                cur = cur->father_;
            }

            while(strcmp(cur->father_->first_child_->node_name_, "Specifier")!=0) cur = cur->father_;
            tail->u.array.elem = cur->father_->first_child_->type_;
        }

        HashTableNode* node = StackTopFind(page_stack, sym);
        if(node==NULL || node->symbol_.kind==FUNCTION){
            HashAdd(StackTop(page_stack), sym);
        }
        else{
            if(judge==0){
                printf("Error type 3 at Line %d: varialbe %s has been declared\n", self->line_, self->compos_.id_);
            }
            else printf("Error type 15 at Line %d: field %s has been declared\n", self->line_, self->compos_.id_);
        }

        if(judge==1 && self->father_->brother_!=NULL && strcmp(self->father_->brother_->node_name_, "ASSIGNOP")==0){
            printf("Error type 15 at Line %d: initialize the field %s in declaration\n", self->line_, self->compos_.id_);
        }

        self->type_ = cur->type_ = sym.u.sym_type;
    }
}

void PreProcess(TreeNode* self){ // handle the stack of symbol tables
    if(strcmp(self->node_name_, "ID")==0 && strcmp(self->father_->node_name_, "FunDec")==0){
        HashTable* new_symbol_table = HashInit();
        StackPush(page_stack, new_symbol_table);

        TreeNode *temp = self->brother_->brother_;
        if(strcmp(temp->node_name_, "VarList")==0) ProcessNode(temp);
        temp = self->father_->brother_;
        assert(strcmp(temp->node_name_, "CompSt")==0);
        ProcessNode(temp);

        StackPop(page_stack);
    }
    else if(strcmp(self->node_name_, "CompSt")==0){
        if(self->father_->cnt_child_ == 1){ // Stmt -> CompSt
            HashTable* new_symbol_table = HashInit();
            StackPush(page_stack, new_symbol_table);
            if(self->cnt_child_ > 2){
                ProcessNode(kthChild(self, 2));
                ProcessNode(kthChild(self, 3));
            }
            StackPop(page_stack);
        }
        else{
            assert(self->father_->cnt_child_ == 3); // function body
        }
    }
}

void CheckSemantics(TreeNode* self){
    if(strcmp(self->node_name_, "Specifier")==0){
        self->type_ = self->first_child_->type_;
    }
    else if(strcmp(self->node_name_, "TYPE")==0){
        self->type_ = (Type*)malloc(sizeof(Type));
        self->type_->kind = BASIC;
        if(strcmp(self->compos_.id_, "int")==0){
            self->type_->u.basic = INTT;
        }
        else if(strcmp(self->compos_.id_, "float")==0){
            self->type_->u.basic = FLOATT;
        }
        else assert(0);
    }
    else if(strcmp(self->node_name_, "StructSpecifier")==0){
        if(self->cnt_child_==5){
            self->type_ = kthChild(self, 4)->type_;
            char *target_name = kthChild(self, 2)->first_child_->compos_.id_;
            // self->type_->u.structure.struct_name = (char*)malloc((strlen(target_name)+1));
            // strcpy(self->type_->u.structure.struct_name, target_name);
            self->type_->u.structure.struct_name = target_name;
        }
        else if(self->cnt_child_==4){
            self->type_ = kthChild(self, 3)->type_;
        }
        else if(self->cnt_child_==2){
            self->type_ = kthChild(self, 2)->type_;
            char *target_name = kthChild(self, 2)->first_child_->compos_.id_;
            // self->type_->u.structure.struct_name = (char*)malloc((strlen(target_name)+1) * sizeof(char));
            // strcpy(self->type_->u.structure.struct_name, target_name);
            self->type_->u.structure.struct_name = target_name;
        }
        else assert(0);
    }
    else if(strcmp(self->node_name_, "DefList")==0 && strcmp(self->father_->node_name_, "DefList")!=0){
        if(self->type_==NULL){
            self->type_ = (Type*)malloc(sizeof(Type));
        }
        self->type_->kind = STRUCTURE;
        Field* tail = NULL;
        TreeNode* cur = self; 

        while(1){
            if(tail==NULL){
                self->type_->u.structure.field = cur->first_child_->type_->u.structure.field;
                tail = self->type_->u.structure.field;
                while(tail->nxt!=NULL) tail = tail->nxt;
            }
            else{
                tail-> nxt = cur->first_child_->type_->u.structure.field;
                while(tail->nxt!=NULL) tail = tail->nxt;
            }

            if(cur->cnt_child_==1) break;
            else cur = kthChild(cur, 2);
        }
    }
    else if(strcmp(self->node_name_, "Def")==0){
        self->type_ = (Type*)malloc(sizeof(Type));
        self->type_->kind = STRUCTURE;

        TreeNode* Dec = kthChild(self, 2); // DecList
        Field* tail = NULL;
        while(1){
            Field* new_fild = (Field*)malloc(sizeof(Field));
            new_fild->nxt = NULL;
            new_fild->type = self->first_child_->type_;
            TreeNode* get_name = Dec;
            while(strcmp(get_name->node_name_, "ID")!=0) get_name = get_name->first_child_;
            // new_fild->name = (char*)malloc(strlen(get_name->compos_.id_)+1);
            // strcpy(new_fild->name, get_name->compos_.id_);
            new_fild->name = get_name->compos_.id_;
            if(tail == NULL){
                self->type_->u.structure.field = new_fild;
                tail = new_fild;
            }
            else{
                tail->nxt = new_fild;
                tail = tail->nxt;
            }
            if(Dec->cnt_child_==1) break;
            else Dec = kthChild(Dec, 3);
        }
    }
    else if(strcmp(self->node_name_, "Tag")==0){
        Symbol sym;
        // sym.name = (char*)malloc(strlen(self->first_child_->compos_.id_)+1);
        // strcpy(sym.name, self->first_child_->compos_.id_);
        sym.name = self->first_child_->compos_.id_;
        sym.kind = STRUCTT;

        HashTableNode* node = StackFind(page_stack, sym);
        if(node==NULL || node->symbol_.kind!=STRUCTT){
            printf("Error type 17 at Line %d: structure %s is undefined\n", self->line_, sym.name);
        }
        else{
            self->type_ = node->symbol_.u.sym_type;
        }
    }
    else if(strcmp(self->node_name_, "Exp")==0){
        if(self->cnt_child_==1){ // EXP -> ID / INT / FLOAT
            if(strcmp(self->first_child_->node_name_, "ID")==0){
                self->assignable_ = 1;
                Symbol sym;
                sym.kind = VARIABLE;
                // sym.name = (char*)malloc(strlen(self->first_child_->compos_.id_)+1);
                // strcpy(sym.name, self->first_child_->compos_.id_);
                sym.name = self->first_child_->compos_.id_;
                HashTableNode* node = StackFind(page_stack, sym);
                if(node==NULL){
                    printf("Error type 1 at Line %d: undeclared variable %s\n", self->line_, sym.name);
                }
                else if(node->symbol_.kind==VARIABLE){
                    self->type_ = node->symbol_.u.sym_type;
                }
                else if(node->symbol_.kind==STRUCTT){
                    printf("Error type 7 at Line %d: the operation is invalid\n", self->line_);
                }
                else assert(0);
            }
            else if(strcmp(self->first_child_->node_name_, "INT")==0){
                self->type_ = (Type*)malloc(sizeof(Type));
                self->type_->kind = BASIC;
                self->type_->u.basic = INTT;
            }
            else if(strcmp(self->first_child_->node_name_, "FLOAT")==0){
                self->type_ = (Type*)malloc(sizeof(Type));
                self->type_->kind = BASIC;
                self->type_->u.basic = FLOATT;
            }
            else assert(0);
        }
        else if(self->cnt_child_==2){
            if(strcmp(self->first_child_->node_name_, "MINUS")==0){ // Exp -> MINUS Exp
                if(kthChild(self, 2)->type_->kind==BASIC){
                    self->type_ = kthChild(self, 2)->type_;
                }
                else printf("Error type 7 at Line %d: the operation is invalid\n", self->line_);
            }
            else if(strcmp(self->first_child_->node_name_, "NOT")==0){
                if(kthChild(self, 2)->type_->kind==BASIC && kthChild(self, 2)->type_->u.basic==INTT){
                    self->type_ = kthChild(self, 2)->type_;
                }
                else printf("Error type 7 at Line %d: the operation is invalid\n", self->line_);
            }
            else assert(0);
        }
        else if(self->cnt_child_==3){
            if(strcmp(kthChild(self, 3)->node_name_, "Exp")==0 && strcmp(kthChild(self, 3)->node_name_, "Exp")==0){ // Exp -> Exp OP Exp
                if(strcmp(kthChild(self, 2)->node_name_, "ASSIGNOP")!=0){
                    TreeNode* lch = kthChild(self, 1);
                    TreeNode* rch = kthChild(self, 3);
                    if(lch->type_->kind==BASIC && rch->type_->kind==BASIC && lch->type_->u.basic==rch->type_->u.basic){
                        self->type_ = lch->type_;
                    }
                    else{
                        printf("Error type 7 at Line %d: the operation is invalid\n", self->line_);
                    }
                }
                else{
                    self->assignable_ = 1;
                    TreeNode* lch = kthChild(self, 1);
                    TreeNode* rch = kthChild(self, 3);

                    if(lch->type_ && rch->type_ && TypeMatch(lch->type_, rch->type_)==0){
                        printf("Error type 5 at Line %d: the operation is invalid\n", self->line_);
                    }
                    if(lch->assignable_==0){
                        printf("Error type 6 at Line %d: the left side of assignment is invalid \n", self->line_);
                    }
                    self->type_ = lch->type_;
                }
            }
            else if(strcmp(self->first_child_->node_name_, "LP")==0){ // Exp -> LP Exp RP
                self->type_ = kthChild(self, 2)->type_;
            }
            else if(strcmp(self->first_child_->node_name_, "ID")==0){ // Exp -> ID LP RP
                Symbol sym;
                // sym.name = (char*)malloc()
                // strcpy(sym.name, self->first_child_->compos_.id_);
                sym.name = self->first_child_->compos_.id_;
                HashTableNode* node = StackFind(page_stack, sym);
                if(node==NULL){
                    printf("Error type 2 at Line %d: undeclared function %s\n", self->line_, sym.name);
                }
                else if(node->symbol_.kind == FUNCTION){
                    self->type_ = node->symbol_.u.sym_func->retn;
                }
                else{
                    printf("Error type 11 at Line %d: %s is not a function\n", self->line_, sym.name);
                }
            }
            else if(strcmp(self->first_child_->node_name_, "Exp")==0){ // Exp -> Exp DOT ID
                self->assignable_ = 1;
                Type* type_struct = self->first_child_->type_;
                if(type_struct->kind!=STRUCTURE){
                    printf("Error type 13 at Line %d: illegal use of \".\" \n", self->line_);
                    return;
                }
                char* fld = kthChild(self, 3)->compos_.id_;
                Field* type_fld = HasFld(type_struct->u.structure.field, fld);
                if(type_fld==NULL){
                    printf("Error type 14 at Line %d: structure doesn't have field %s\n", self->line_, fld);
                }
                else{
                    Symbol sym;
                    // strcpy(sym.name, kthChild(self, 3)->compos_.id_);
                    sym.name = kthChild(self, 3)->compos_.id_;
                    HashTableNode *node = StackFind(page_stack, sym);
                    assert(node!=NULL);
                    self->type_ = node->symbol_.u.sym_type;
                }
            }
            else assert(0);
        }
        else if(self->cnt_child_==4){
            if(strcmp(self->first_child_->node_name_, "ID")==0){ // Exp -> ID LP Args RP
                Symbol sym;
                // strcpy(sym.name, self->first_child_->compos_.id_);
                sym.name = self->first_child_->compos_.id_;
                HashTableNode* node = StackFind(page_stack, sym);
                if(node==NULL){
                    printf("Error type 2 at Line %d: undeclared function %s\n", self->line_, sym.name);
                }
                else if(node->symbol_.kind == FUNCTION){
                    if(strcmp(node->symbol_.name, "write")!=0){
                        self->type_ = node->symbol_.u.sym_func->retn;

                        Field* para = node->symbol_.u.sym_func->Argv;
                        TreeNode* arg = kthChild(self, 3);


                        while(para->nxt!=NULL && arg->cnt_child_==3 && TypeMatch(para->type, arg->first_child_->type_)){
                            para = para->nxt;
                            arg = kthChild(arg, 3);
                        }

                        if(para->nxt==NULL && arg->cnt_child_==1 && TypeMatch(para->type, arg->first_child_->type_)){
                            self->type_ = node->symbol_.u.sym_func->retn;
                        }
                        else printf("Error type 9 at Line %d: parameters are not matched\n", self->line_);
                    }
                }
                else{
                    printf("Error type 11 at Line %d: %s is not a function\n", self->line_, sym.name);
                }
            }
            else if(strcmp(self->first_child_->node_name_, "Exp")==0){ // Exp -> Exp LB Exp RB
                self->assignable_ = 1;
                if(self->first_child_->type_->kind != ARRAY){
                    printf("Error type 10 at Line %d: no array\n", self->line_);
                }
                else{
                    Type* type_idx = kthChild(self, 3)->type_;
                    if(type_idx->kind!=BASIC || type_idx->u.basic!=INTT){
                        printf("Error type 12 at Line %d: index is not int\n", self->line_);
                    }
                    else self->type_ = self->first_child_->type_->u.array.elem;
                }
            }
        }
        else {
            assert(0);
        }


        if(strcmp(self->father_->first_child_->node_name_, "RETURN")==0){
            TreeNode* tmp = self;
            while(strcmp(tmp->node_name_, "ExtDef")!=0) tmp = tmp->father_;
            if(!TypeMatch(tmp->first_child_->type_, self->type_)){
                printf("Error type 8 at Line %d: return type does not match\n", self->line_);
            }
        }

        if(strcmp(self->father_->node_name_, "Dec")==0){
            TreeNode* check = self;
            while(strcmp(check->node_name_, "Def")!=0) check = check->father_;
            check = check->first_child_;
            assert(strcmp(check->node_name_, "Specifier")==0);
            if(!TypeMatch(check->type_, self->type_)){
                printf("Error type 5 at Line %d: the operation is invalid\n", self->line_);
            }
        }
    }
    else if(strcmp(self->node_name_, "ParamDec")==0){
        self->type_ = kthChild(self, 2)->type_;
    }
}

void ProcessNode(TreeNode* self){
    if(self == NULL) return;
    if(self->anaylised_ == 1) return;
    self->anaylised_ = 1;

    PreProcess(self); // handle the stack of symbol tables

    // recursively deduce the type
    TreeNode* cur = self->first_child_;
    while(cur != NULL){
        ProcessNode(cur);
        cur = cur->brother_;
    }

    CheckSemantics(self);
    InsertSymbol(self);

    if(self->type_ == NULL){ // if it is NULL, sometimes(there's error in .tmm) it causes FAULT
        self->type_ = (Type*)malloc(sizeof(Type));
    }
}

int Hex2Dec(char ch){
    if(ch>='0' && ch<='9'){
        return ch - '0';
    }
    else if(ch>='a' && ch<='f'){
        return ch - 'a' + 10;
    }
    else if(ch>='A' && ch<='F'){
        return ch - 'A' + 10;
    }
    else{
        printf("wrong hex %c\n", ch);
        assert(0);
    }
}

TreeNode* TreeNodeInit(char node_name[], int line, char text[]){
    TreeNode* self = (TreeNode*)malloc(sizeof(TreeNode));
    self->node_name_ = (char*)malloc(strlen(node_name)+1);
    strcpy(self->node_name_, node_name);
    self->line_ = line;
    self->cnt_child_ = 0;
    self->father_ = NULL;
    self->first_child_ = NULL;
    self->brother_ = NULL;

    if(strcmp(node_name, "DECE")==0){
        self->compos_.val_int_ = atoi(text);
        strcpy(self->node_name_, "INT");
    }
    else if(strcmp(node_name, "OCT")==0){
        int l = strlen(text)-1;
        int res = 0, base = 1;
        while(l){
            res += (text[l]-'0') * base;
            base *= 8;
            l--;
        }
        self->compos_.val_int_ = res;
        strcpy(self->node_name_, "INT");
    }
    else if(strcmp(node_name, "HEX")==0){
        int l = strlen(text)-1;
        int res = 0, base = 1;
        while(l>1){
            res += Hex2Dec(text[l])*base;
            base *= 16;
            l--;
        }
        self->compos_.val_int_ = res;
        strcpy(self->node_name_, "INT");
    }
    else if(strcmp(node_name, "FLOAT")==0){
        self->compos_.val_float_ = (float)atof(text);
    }
    else{
        self->compos_.id_ = (char*)malloc(strlen(text)+1);
        strcpy(self->compos_.id_, text);
    }

    return self;
}

TreeNode* Fatherize(char node_name[], int line, int cnt_child, TreeNode* children[7]){
    assert(cnt_child>=1);
    TreeNode* self = (TreeNode*)malloc(sizeof(TreeNode));
    self->node_name_ = (char*)malloc(strlen(node_name)+1);
    strcpy(self->node_name_, node_name);
    self->line_ = line;
    self->father_ = NULL;
    self->brother_ = NULL;
    self->compos_.val_int_ = 0;
    self->first_child_ = children[0];
    self->cnt_child_ = 1;

    TreeNode* cur = self->first_child_;
    cur->father_ = self;
    assert(cur->brother_ == NULL);
    for(int i=1; i<cnt_child; i++){
        if(children[i]==NULL) continue;
        cur->brother_ = children[i];
        cur = cur->brother_;
        cur->father_ = self;
        self->cnt_child_++;
        assert(cur->brother_ == NULL);
    }

    return self;
}

TreeNode* kthChild(TreeNode* self, int k){
    assert(k <= self->cnt_child_ && k >= 1);
    TreeNode* cur = self->first_child_;
    int now = 1;
    while(now != k){
        cur = cur->brother_;
        now++;
    }
    return cur;
}

ParseTree* ParseTreeInit(TreeNode* root){
    ParseTree* self = (ParseTree*)malloc(sizeof(ParseTree));
    self->root_ = root;
    return self;
}

void PrintNodes(TreeNode* node, int depth){
    for(int i=0; i<depth*2; i++){
        printf(" ");
    }

    if(node->first_child_ == NULL){ // leaf node
        printf("%s", node->node_name_);
        if(strcmp(node->node_name_, "ID")==0) printf(": %s\n",node->compos_.id_);
        else if(strcmp(node->node_name_, "INT")==0) printf(": %d\n",node->compos_.val_int_);
        else if(strcmp(node->node_name_, "FLOAT")==0) printf(": %f\n",node->compos_.val_float_);
        else if(strcmp(node->node_name_, "TYPE")==0) printf(": %s\n",node->compos_.id_);
        else printf("\n");
    }
    else{
        printf("%s (%d)\n", node->node_name_, node->line_);
        TreeNode* cur = node->first_child_;
        while(cur != NULL){
            PrintNodes(cur, depth+1);
            cur = cur->brother_;
        }
    }
}

void PrintTree(ParseTree* self){
    PrintNodes(self->root_, 0);
}

void ProcessTree(ParseTree* self){
    ProcessNode(self->root_);
}