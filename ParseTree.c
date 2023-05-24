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

void Check(struct tree_node* root){
    if(root==NULL) return;
    if(root->anaylised == 1) return;

    root->anaylised = 1;


    if(strcmp(root->name, "ID")==0 && strcmp(root->father->name, "FunDec")==0){
        struct tree_node *temp = root->brother->brother;
        if(strcmp(temp->name, "VarList")==0) Check(temp);
    }

    struct tree_node* curr = root->first_child;
    while(curr!=NULL){
        Check(curr);
        curr = curr->brother;
    }

    // printf("%s in\n", root->name);

    if(strcmp(root->name, "Specifier")==0){
        root->type = root->first_child->type;
    }
    else if(strcmp(root->name, "TYPE")==0){
        root->type = (Type*)malloc(sizeof(Type));
        root->type->kind = BASEE;
        if(strcmp(root->compos.id, "int")==0){
            root->type->type.base = INTT;
        }
        else if(strcmp(root->compos.id, "float")==0) root->type->type.base = FLOATT;
        else assert(0);
    }
    else if(strcmp(root->name, "StructSpecifier")==0){
        if(root->cnt_child==5) root->type = child_of_no(4, root)->type;
        else if(root->cnt_child==4) root->type = child_of_no(3, root)->type;
        else if(root->cnt_child==2) root->type = child_of_no(2, root)->type;
        else assert(0);
    }
    else if(strcmp(root->name, "DefList")==0 && strcmp(root->father->name, "DefList")!=0){
        if(root->type==NULL){
            root->type = (Type*)malloc(sizeof(Type));
        }
        root->type->kind = STRUCTT;
        Field* tail = NULL;
        struct tree_node* cur = root; 

        while(1){
            if(tail==NULL){
                root->type->type.struc = cur->first_child->type->type.struc;
                tail = root->type->type.struc;
                while(tail->nxt!=NULL) tail = tail->nxt;
            }
            else{
                tail-> nxt = cur->first_child->type->type.struc;
                while(tail->nxt!=NULL) tail = tail->nxt;
            }

            if(cur->cnt_child==1) break;
            else cur = child_of_no(2, cur);
        }
    }
    else if(strcmp(root->name, "Def")==0){
        root->type = (Type*)malloc(sizeof(Type));
        root->type->kind = STRUCTT;

        struct tree_node* Dec = child_of_no(2, root); // DecList
        Field* tail = NULL;
        while(1){
            Field* new_fild = (Field*)malloc(sizeof(Field));
            new_fild->nxt = NULL;
            new_fild->type_field = root->first_child->type;
            struct tree_node* get_name = Dec;
            while(strcmp(get_name->name, "ID")!=0) get_name = get_name->first_child;
            strcpy(new_fild->name, get_name->compos.id);
            if(tail == NULL){
                root->type->type.struc = new_fild;
                tail = new_fild;
            }
            else{
                tail->nxt = new_fild;
                tail = tail->nxt;
            }
            if(Dec->cnt_child==1) break;
            else Dec = child_of_no(3, Dec);
        }
    }
    else if(strcmp(root->name, "Tag")==0){
        Symbol sym;
        strcpy(sym.name, root->first_child->compos.id);
        sym.kind = STRUCTT;

        HashTableNode* node = Hash_Find(&Hash_table, sym);
        if(node==NULL || node->symbol.kind!=STRUCTT){
            printf("Error type 17 at Line %d: structure %s is undefined\n", root->first_line, sym.name);
        }
        else{
            root->type = node->symbol.prop.sym_type;
        }
    }
    else if(strcmp(root->name, "Exp")==0){
        if(root->cnt_child==1){ // EXP -> ID / INT / FLOAT
            if(strcmp(root->first_child->name, "ID")==0){
                root->assignable = 1;
                Symbol sym;
                sym.kind = VARIABLEE;
                strcpy(sym.name, root->first_child->compos.id);
                HashTableNode* node = Hash_Find(&Hash_table, sym);
                if(node==NULL){
                    printf("Error type 1 at Line %d: undeclared variable %s\n", root->first_line, sym.name);
                }
                else if(node->symbol.kind==VARIABLEE){
                    root->type = node->symbol.prop.sym_type;
                }
                else if(node->symbol.kind==STRUCTT){
                    printf("Error type 7 at Line %d: the operation is invalid\n", root->first_line);
                }
                else assert(0);
            }
            else if(strcmp(root->first_child->name, "INT")==0){
                root->type = (Type*)malloc(sizeof(Type));
                root->type->kind = BASEE;
                root->type->type.base = INTT;
            }
            else if(strcmp(root->first_child->name, "FLOAT")==0){
                root->type = (Type*)malloc(sizeof(Type));
                root->type->kind = BASEE;
                root->type->type.base = FLOATT;
            }
            else assert(0);
        }
        else if(root->cnt_child==2){
            if(strcmp(root->first_child->name, "MINUS")==0){ // Exp -> MINUS Exp
                if(child_of_no(2, root)->type->kind==BASEE){
                    root->type = child_of_no(2, root)->type;
                }
                else printf("Error type 7 at Line %d: the operation is invalid\n", root->first_line);
            }
            else if(strcmp(root->first_child->name, "NOT")==0){
                if(child_of_no(2, root)->type->kind==BASEE && child_of_no(2, root)->type->type.base==INTT){
                    root->type = child_of_no(2, root)->type;
                }
                else printf("Error type 7 at Line %d: the operation is invalid\n", root->first_line);
            }
            else assert(0);
        }
        else if(root->cnt_child==3){
            if(strcmp(child_of_no(3, root)->name, "Exp")==0 && strcmp(child_of_no(3, root)->name, "Exp")==0){ // Exp -> Exp OP Exp
                if(strcmp(child_of_no(2, root)->name, "ASSIGNOP")!=0){
                    struct tree_node* lch = child_of_no(1, root);
                    struct tree_node* rch = child_of_no(3, root);
                    if(lch->type->kind==BASEE && rch->type->kind==BASEE && lch->type->type.base==rch->type->type.base){
                        root->type = lch->type;
                    }
                    else{
                        printf("Error type 7 at Line %d: the operation is invalid\n", root->first_line);
                    }
                }
                else{
                    root->assignable = 1;
                    struct tree_node* lch = child_of_no(1, root);
                    struct tree_node* rch = child_of_no(3, root);

                    if(lch->type && rch->type && TypeMatch(lch->type, rch->type)==0){
                        printf("Error type 5 at Line %d: the operation is invalid\n", root->first_line);
                    }
                    if(lch->assignable==0){
                        printf("Error type 6 at Line %d: the left side of assignment is invalid \n", root->first_line);
                    }
                }
            }
            else if(strcmp(root->first_child->name, "LP")==0){ // Exp -> LP Exp RP
                root->type = root->first_child->type;
            }
            else if(strcmp(root->first_child->name, "ID")==0){ // Exp -> ID LP RP
                Symbol sym;
                strcpy(sym.name, root->first_child->compos.id);
                HashTableNode* node = Hash_Find(&Hash_table, sym);
                if(node==NULL){
                    printf("Error type 2 at Line %d: undeclared function %s\n", root->first_line, sym.name);
                }
                else if(node->symbol.kind == FUNCTIONN){
                    root->type = node->symbol.prop.sym_func->retn;
                }
                else{
                    printf("Error type 11 at Line %d: %s is not a function\n", root->first_line, sym.name);
                }
            }
            else if(strcmp(root->first_child->name, "Exp")==0){ // Exp -> Exp DOT ID
                root->assignable = 1;
                Type* type_struct = root->first_child->type;
                if(type_struct->kind!=STRUCTT){
                    printf("Error type 13 at Line %d: illegal use of \".\" \n", root->first_line);
                    return;
                }
                char* fld = child_of_no(3, root)->compos.id;
                Field* type_fld = HasFld(type_struct->type.struc, fld);
                if(type_fld==NULL){
                    printf("Error type 14 at Line %d: structure doesn't have fild %s\n", root->first_line, fld);
                }
                else{
                    Symbol sym;
                    strcpy(sym.name, child_of_no(3, root)->compos.id);
                    HashTableNode *node = Hash_Find(&Hash_table, sym);
                    assert(node!=NULL);
                    root->type = node->symbol.prop.sym_type;
                }
            }
            else assert(0);
        }
        else if(root->cnt_child==4){
            if(strcmp(root->first_child->name, "ID")==0){ // Exp -> ID LP Args RP
                Symbol sym;
                strcpy(sym.name, root->first_child->compos.id);
                HashTableNode* node = Hash_Find(&Hash_table, sym);
                if(node==NULL){
                    printf("Error type 2 at Line %d: undeclared function %s\n", root->first_line, sym.name);
                }
                else if(node->symbol.kind == FUNCTIONN){
                    if(strcmp(node->symbol.name, "write")!=0){

                        Field* para = node->symbol.prop.sym_func->Argv;
                        struct tree_node* arg = child_of_no(3, root);


                        while(para->nxt!=NULL && arg->cnt_child==3 && TypeMatch(para->type_field, arg->first_child->type)){
                            para = para->nxt;
                            arg = child_of_no(3, arg);
                        }

                        if(para->nxt==NULL && arg->cnt_child==1 && TypeMatch(para->type_field, arg->first_child->type)){
                            root->type = node->symbol.prop.sym_func->retn;
                        }
                        else printf("Error type 9 at Line %d: parameters are not matched\n", root->first_line);
                    }
                }
                else{
                    printf("Error type 11 at Line %d: %s is not a function\n", root->first_line, sym.name);
                }
            }
            else if(strcmp(root->first_child->name, "Exp")==0){ // Exp -> Exp LB Exp RB
                root->assignable = 1;
                if(root->first_child->type->kind != ARRAYY){
                    printf("Error type 10 at Line %d: no array\n", root->first_line);
                }
                else{
                    Type* type_idx = child_of_no(3, root)->type;
                    if(type_idx->kind!=BASEE || type_idx->type.base!=INTT){
                        printf("Error type 12 at Line %d: index is not int\n", root->first_line);
                    }
                    else root->type = root->first_child->type->type.array.type_ele;
                }
            }
        }
        else {
            assert(0);
        }


        if(strcmp(root->father->first_child->name, "RETURN")==0){
            struct tree_node* tmp = root;
            while(strcmp(tmp->name, "ExtDef")!=0) tmp = tmp->father;
            if(!TypeMatch(tmp->first_child->type, root->type)){
                printf("Error type 8 at Line %d: return type does not match\n", root->first_line);
            }
        }

        if(strcmp(root->father->name, "Dec")==0){
            struct tree_node* check = root;
            while(strcmp(check->name, "Def")!=0) check = check->father;
            check = check->first_child;
            assert(strcmp(check->name, "Specifier")==0);
            if(!TypeMatch(check->type, root->type)){
                printf("Error type 5 at Line %d: the operation is invalid\n", root->first_line);
            }
        }
    }
    else if(strcmp(root->name, "ParamDec")==0){
        root->type = child_of_no(2, root)->type;
    }
    
    Insert(root);

    if(root->type == NULL){ // if it is NULL, sometimes(there's error in .tmm) it causes FAULT
        root->type = (Type*)malloc(sizeof(Type));
    }

    // printf("%s out\n", root->name);
    

}


void Insert(struct tree_node* root){
    struct tree_node* cur = root;
    if(strcmp(cur->name, "ID")!=0) return;

    Symbol sym;
    strcpy(sym.name, cur->compos.id);

    if(strcmp(cur->father->name, "OptTag")==0){
        sym.kind = STRUCTT;

        // declare a struct 
        struct tree_node* DefLst = cur->father->brother->brother;
        DefLst->type = (Type*)malloc(sizeof(Type));
        assert(strcmp(DefLst->name, "DefList")==0);
        sym.prop.sym_type = DefLst->type;

        HashTableNode* node = Hash_Find(&Hash_table, sym);
        if(node==NULL || node->symbol.kind==FUNCTIONN){
            Hash_Add(&Hash_table, sym);
        }
        else{
            printf("Error type 16 at Line %d: name of structure %s is repeated\n", cur->first_line, cur->compos.id);
        }
    }
    else if(strcmp(cur->father->name, "FunDec")==0){
        cur = cur->father;
        sym.kind = FUNCTIONN;
        
        // declare a function
        sym.prop.sym_func = (Func*)malloc(sizeof(Func));
        sym.prop.sym_func->retn = cur->father->first_child->type;
        sym.prop.sym_func->Argc_cnt = 0;
        sym.prop.sym_func->Argv = NULL;

        if(cur->cnt_child==4){ // ID LP VarList RP
            struct tree_node* para = child_of_no(3, cur);
            Field* tail = NULL;

            while(1){
                assert(strcmp(para->name, "VarList")==0);
                sym.prop.sym_func->Argc_cnt++;
                
                Field* new_para = (Field*)malloc(sizeof(Field));
                new_para->type_field = child_of_no(2, para->first_child)->type;
                new_para->nxt = NULL;

                if(tail==NULL){
                    tail = new_para;
                    sym.prop.sym_func->Argv = new_para;
                }
                else{
                    tail->nxt = new_para;
                    tail = tail->nxt;
                }


                if(para->cnt_child == 1) break;
                else para = child_of_no(3, para);
            }
        };


        HashTableNode* node = Hash_Find(&Hash_table, sym);
        if(node==NULL || node->symbol.kind!=FUNCTIONN){
            Hash_Add(&Hash_table, sym);
        }
        else{
            printf("Error type 4 at Line %d: function %s has been declared\n", cur->first_line, cur->compos.id);
        }
    }
    else if(strcmp(cur->father->name, "VarDec")==0){
        struct tree_node* check = cur;
        int judge = VARIABLEE;
        while(check!=NULL && strcmp(check->name, "StructSpecifier")!=0) check = check->father;

        if(check!=NULL){ // a field of a struct
            judge = STRUCTT;
            // Symbol sym;
            // sym.kind = VARIABLEE;
            // strcpy(sym.name, cur->compos.id);

            // HashTableNode* node = Hash_Find(&Hash_table, sym);
            // if(node==NULL || node->symbol.kind==FUNCTIONN){
            //     Hash_Add(&Hash_table, sym);
            // }
            // else{
            //     printf("Error type 15 at Line %d: field %s has been declared\n", cur->first_line, cur->compos.id);
            // }

            // if(cur->father->brother!=NULL && strcmp(cur->father->brother->name, "ASSIGNOP")==0){
            //     printf("Error type 15 at Line %d: initialize the field %s in declaration\n", cur->first_line, cur->compos.id);
            // }
            // return;
        }

        sym.kind = VARIABLEE;
        // declare a variable
        if(cur->father->brother==NULL || strcmp(cur->father->brother->name, "LB")!=0){
            while(strcmp(cur->father->first_child->name, "Specifier")!=0) cur = cur->father;
            sym.prop.sym_type = cur->father->first_child->type;
        }
        else{
            cur = cur->father;
            sym.prop.sym_type = (Type*)malloc(sizeof(Type));
            sym.prop.sym_type->kind = ARRAYY;

            Type* tail = NULL;

            while(strcmp(cur->father->name, "VarDec")==0){
                Type* Dem = (Type*)malloc(sizeof(Type));
                Dem->kind = ARRAYY;
                Dem->type.array.size = child_of_no(3, cur->father)->compos.val_int;
                Dem->type.array.type_ele = NULL;
                if(tail == NULL){
                    tail = Dem;
                    sym.prop.sym_type = Dem;
                }
                else{
                    tail->type.array.type_ele = Dem;
                    tail = Dem;
                }
                cur = cur->father;
            }

            while(strcmp(cur->father->first_child->name, "Specifier")!=0) cur = cur->father;
            tail->type.array.type_ele = cur->father->first_child->type;
        }

        HashTableNode* node = Hash_Find(&Hash_table, sym);
        if(node==NULL || node->symbol.kind==FUNCTIONN){
            Hash_Add(&Hash_table, sym);
        }
        else{
            if(judge==VARIABLEE){
                printf("Error type 3 at Line %d: varialbe %s has been declared\n", cur->first_line, cur->compos.id);
            }
            else printf("Error type 15 at Line %d: field %s has been declared\n", cur->first_line, cur->compos.id);
        }

        if(judge==STRUCTT && cur->father->brother!=NULL && strcmp(cur->father->brother->name, "ASSIGNOP")==0){
            printf("Error type 15 at Line %d: initialize the field %s in declaration\n", cur->first_line, cur->compos.id);
        }

        root->type = cur->type = sym.prop.sym_type;
    }
}


int ExistStruct(struct tree_node *root){
    if(strcmp(root->name, "STRUCT")==0){
        return 1;
    }

    struct tree_node *cur = root->first_child;
    while(cur!=NULL){
        if(ExistStruct(cur)==1) return 1;
        else cur = cur->brother;
    }
    return 0;
}