// #include "translate.h"
// #include "Stack.h"

// extern int vari_cnt;
// extern int func_cnt;
// extern int array_cnt;
// extern int label_cnt;
// extern int add_cnt;
// extern InterCodeNode *CodeList;
// extern Stack* page_stack;

// Operand new_label(){
//     Operand new;
//     new.kind = OP_LABEL;
//     new.u.no_label = ++label_cnt;
//     return new;
// }

// Operand new_temp(){
//     Operand new;
//     new.kind = OP_VARI;
//     new.u.no_label = ++vari_cnt;
//     return new;
// }

// Operand new_const(int x){
//     Operand new;
//     new.kind = OP_CONST;
//     new.u.val_const = x;
//     return new;
// }

// Operand new_add(){
//     Operand new;
//     new.kind = OP_ADD;
//     new.u.no_add = ++add_cnt;
//     return new;
// }

// Operand empty;
// int size = 0;
// char rel[3] = "";

// void Trans_Exp(struct tree_node *root, Operand opde){
//     assert(strcmp(root->name, "Exp")==0);

//     Operand op1, op2, op3;

//     if(root->cnt_child==1){
//         if(strcmp(root->first_child->name, "INT")==0){
//             op2.kind = OP_CONST;
//             op2.u.val_const = root->first_child->compos.val_int;
//             Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, op2, op3, size, rel));
//         }
//         else if(strcmp(root->first_child->name, "ID")==0){
//             Symbol sym;
//             strcpy(sym.name, root->first_child->compos.id);
//             HashTableNode *cur = Stack_find(page_stack, sym);
//             op2.kind = OP_VARI;
//             assert(cur!=NULL);
//             op2.u.no_vari = cur->symbol.rank;
//             Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, op2, op3, size, rel));
//         }
//     }
//     else if(root->cnt_child==2){
//         if(strcmp(root->first_child->name, "MINUS")==0){
//             Operand temp_op;
//             temp_op.kind = OP_VARI;
//             temp_op.u.no_vari = ++vari_cnt;

//             Trans_Exp(child_of_no(2, root), temp_op);

//             op2.kind = OP_CONST;
//             op2.u.val_const = 0;
//             Add_InterCode(CodeList, Gen_Code(SUB, opde, op2, temp_op, size, rel));
//         }
//         else if(strcmp(root->first_child->name, "NOT")==0){
//             op2.kind = OP_CONST;
//             op2.u.val_const = 0;
//             Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, op2, op3, size, rel));

//             op1.kind = OP_LABEL;
//             op1.u.no_label = ++label_cnt;
//             op2.kind = OP_LABEL;
//             op2.u.no_label = ++label_cnt;
//             Trans_Cond(root, op1, op2);

//             Add_InterCode(CodeList, Gen_Code(LABEL, op1, op2, op3, size, rel));
            
//             op3.kind = OP_CONST;
//             op3.u.val_const = 1;
//             Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, op3, op3, size, rel));

//             Add_InterCode(CodeList, Gen_Code(LABEL, op2, op2, op3, size, rel));
//         }
//     }
//     else if(root->cnt_child==3){
//         char midname[20];
//         strcpy(midname, child_of_no(2, root)->name);

//         if(strcmp(midname, "RELOP")==0 || strcmp(midname, "AND")==0 || strcmp(midname, "OR")==0){
//             op2.kind = OP_CONST;
//             op2.u.val_const = 0;
//             Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, op2, op3, size, rel));

//             op1.kind = OP_LABEL;
//             op1.u.no_label = ++label_cnt;
//             op2.kind = OP_LABEL;
//             op2.u.no_label = ++label_cnt;
//             Trans_Cond(root, op1, op2);

//             Add_InterCode(CodeList, Gen_Code(LABEL, op1, op2, op3, size, rel));
            
//             op3.kind = OP_CONST;
//             op3.u.val_const = 1;
//             Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, op3, op3, size, rel));

//             Add_InterCode(CodeList, Gen_Code(LABEL, op2, op2, op3, size, rel));
//         }
//         else if(strcmp(midname, "ASSIGNOP")==0){
//             if(root->first_child->cnt_child==1 && strcmp(root->first_child->first_child->name, "ID")==0){
//                 Symbol sym;
//                 strcpy(sym.name, root->first_child->first_child->compos.id);
//                 HashTableNode *cur = Stack_find(page_stack, sym);
//                 op1.kind = OP_VARI;
//                 op1.u.no_vari = cur->symbol.rank;

//                 op2.kind = OP_VARI;
//                 op2.u.no_vari = ++vari_cnt;

//                 Trans_Exp(child_of_no(3, root), op2);

//                 Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, op1, op2, op3, size, rel));
//                 Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, op1, op3, size, rel));
//             }
//             else if(root->first_child->cnt_child==4 && strcmp(child_of_no(2, root->first_child)->name, "LB")==0){
//                 Operand addr = new_add();
//                 Trans_Array(root->first_child, addr);

//                 Operand temp = new_temp();
//                 Trans_Exp(child_of_no(3, root), temp);
                
//                 Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2PNT, addr, temp, empty, size, rel));
//             }
//         }
//         else if(strcmp(midname, "LP")==0){
//             Symbol sym;
//             strcpy(sym.name, root->first_child->compos.id);
//             HashTableNode *cur = Stack_find(page_stack, sym);

//             if(strcmp(cur->symbol.name, "read")==0){
//                 Add_InterCode(CodeList, Gen_Code(READ, opde, empty, empty, size, rel));
//             }
//             else{
//                 Operand func;
//                 func.kind = OP_FUNC;
//                 strcpy(func.u.name_func, cur->symbol.name);
//                 Add_InterCode(CodeList, Gen_Code(CALL, opde, func, empty, size, rel));
//             }
//         }
//         else if(strcmp(midname, "Exp")==0){
//             Trans_Exp(child_of_no(2, root), opde);
//         }
//         else{
//             op1 = new_temp();
//             op2 = new_temp();
//             Trans_Exp(child_of_no(1, root), op1);
//             Trans_Exp(child_of_no(3, root), op2);
//             if(strcmp(midname, "PLUS")==0){
//                 Add_InterCode(CodeList, Gen_Code(ADD, opde, op1, op2, size, rel));
//             }
//             else if(strcmp(midname, "MINUS")==0){
//                 Add_InterCode(CodeList, Gen_Code(SUB, opde, op1, op2, size, rel));
//             }
//             else if(strcmp(midname, "STAR")==0){
//                 Add_InterCode(CodeList, Gen_Code(MULTI, opde, op1, op2, size, rel));
//             }
//             else if(strcmp(midname, "DIV")==0){
//                 Add_InterCode(CodeList, Gen_Code(DIV, opde, op1, op2, size, rel));
//             }
//             else assert(0);
//         }
//     }
//     else if(root->cnt_child==4){
//         if(strcmp(root->first_child->name, "ID")==0){ // call a function
//             Symbol sym;
//             strcpy(sym.name, root->first_child->compos.id);
//             HashTableNode *symnode = Stack_find(page_stack, sym);

//             listnode* arg_list = NULL;
//             Trans_Args(child_of_no(3, root), &arg_list);

//             if(strcmp(root->first_child->compos.id, "write")==0){
//                 Add_InterCode(CodeList, Gen_Code(WRITE, arg_list->para, empty, empty, size, rel));
//                 Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, new_const(0), empty, size, rel));
//             }
//             else{
//                 listnode *cur = arg_list;
//                 while(cur!=NULL){
//                     Add_InterCode(CodeList, Gen_Code(ARG, cur->para, empty, empty, size, rel));
//                     cur = cur->next;
//                 }
//                 Operand func;
//                 func.kind = OP_FUNC;
//                 strcpy(func.u.name_func, symnode->symbol.name);
//                 Add_InterCode(CodeList, Gen_Code(CALL, opde, func, empty, size, rel));
//             }
//         }
//         else if(strcmp(child_of_no(2, root)->name, "LB")==0){ // visit an array
//             Operand element = new_add();
//             Trans_Array(root, element);
//             Add_InterCode(CodeList, Gen_Code(ASSIGN_PNT2VAL, opde, element, empty, size, rel));
//         }
//     }
// }

// void Trans_Stmt(struct tree_node* root){
//     assert(strcmp(root->name, "Stmt")==0);

//     if(root->cnt_child==1 && strcmp(root->first_child->name, "CompSt")==0){
//         Trans_CompSt(root->first_child);
//     }
//     else if(root->cnt_child==2 && strcmp(root->first_child->name, "Exp")==0){
//         Trans_Exp(root->first_child, empty);
//     }
//     else if(root->cnt_child==3 && strcmp(root->first_child->name, "RETURN")==0){
//         Operand t1;
//         t1.kind = OP_VARI;
//         t1.u.no_vari = ++vari_cnt;
//         Trans_Exp(child_of_no(2, root), t1);
//         Add_InterCode(CodeList, Gen_Code(RETURN, t1, empty, empty, size, rel));
//     }
//     else if(root->cnt_child==5 && strcmp(root->first_child->name, "IF")==0){
//         Operand l1 = new_label();
//         Operand l2 = new_label();
//         Trans_Cond(child_of_no(3, root), l1, l2);
//         Add_InterCode(CodeList, Gen_Code(LABEL, l1, empty, empty, size, rel));
//         Trans_Stmt(child_of_no(5, root));
//         Add_InterCode(CodeList, Gen_Code(LABEL, l2, empty, empty, size, rel));
//     }
//     else if(root->cnt_child==7){
//         Operand l1 = new_label();
//         Operand l2 = new_label();
//         Operand l3 = new_label();

//         Trans_Cond(child_of_no(3, root), l1, l2);
//         Add_InterCode(CodeList, Gen_Code(LABEL, l1, empty, empty, size, rel));
//         Trans_Stmt(child_of_no(5, root));
//         Add_InterCode(CodeList, Gen_Code(GOTO, l3, empty, empty, size, rel));
//         Add_InterCode(CodeList, Gen_Code(LABEL, l2, empty, empty, size, rel));
//         Trans_Stmt(child_of_no(7, root));
//         Add_InterCode(CodeList, Gen_Code(LABEL, l3, empty, empty, size, rel));
//     }
//     else if(root->cnt_child==5 && strcmp(root->first_child->name, "WHILE")==0){
//         Operand l1 = new_label();
//         Operand l2 = new_label();
//         Operand l3 = new_label();

//         Add_InterCode(CodeList, Gen_Code(LABEL, l1, empty, empty, size, rel));
//         Trans_Cond(child_of_no(3, root), l2, l3);
//         Add_InterCode(CodeList, Gen_Code(LABEL, l2, empty, empty, size, rel));
//         Trans_Stmt(child_of_no(5, root));
//         Add_InterCode(CodeList, Gen_Code(GOTO, l1, empty, empty, size, rel));
//         Add_InterCode(CodeList, Gen_Code(LABEL, l3, empty, empty, size, rel));
//     }
// }

// void Trans_Cond(struct tree_node *root, Operand lt, Operand lf){
//     assert(strcmp(root->name, "Exp")==0);

//     if(root->cnt_child==3){
//         char midname[10];
//         strcpy(midname, child_of_no(2, root)->name);
//         if(strcmp(midname, "RELOP")==0){
//             Operand t1 = new_temp();
//             Operand t2 = new_temp();
//             Trans_Exp(child_of_no(1, root), t1);
//             Trans_Exp(child_of_no(3, root), t2);
//             strcpy(rel, child_of_no(2, root)->compos.id);
//             Add_InterCode(CodeList, Gen_Code(GOTO_COND, t1, t2, lt, size, rel));
//             Add_InterCode(CodeList, Gen_Code(GOTO, lf, empty, empty, size, rel));
//         }
//         else if(strcmp(midname, "AND")==0){
//             Operand l1 = new_label();
//             Trans_Cond(child_of_no(1, root), l1, lf);
//             Add_InterCode(CodeList, Gen_Code(LABEL, l1, empty, empty, size, rel));
//             Trans_Cond(child_of_no(3, root), lt, lf);
//         }
//         else if(strcmp(midname, "OR")==0){
//             Operand l1 = new_label();
//             Trans_Cond(child_of_no(1, root), lt, l1);
//             Add_InterCode(CodeList, Gen_Code(LABEL, l1, empty, empty, size, rel));
//             Trans_Cond(child_of_no(3, root), lt, lf);
//         }
//     }
//     else if(root->cnt_child==2 && strcmp(root->first_child->name, "NOT")==0){
//         Trans_Cond(child_of_no(2, root), lf, lt);
//     }
//     else{
//         Operand t1 = new_temp();
//         Trans_Exp(root, t1);

//         strcpy(rel, "!=");
//         Add_InterCode(CodeList, Gen_Code(GOTO_COND, t1, new_const(0), lt, size, rel));
//         Add_InterCode(CodeList, Gen_Code(GOTO, lf, empty, empty, size, rel));
//     }
// }

// void Translate(struct tree_node *root){

//     // printf("%s in\n", root->name);


//     if(strcmp(root->name, "Program")==0){
//         Translate(root->first_child);
//     }
//     else if(strcmp(root->name, "ExtDefList")==0){
//         Translate(root->first_child);
//         if(root->cnt_child == 2) Translate(child_of_no(2, root));
//     }
//     else if(strcmp(root->name, "ExtDef")==0){
//         if(root->cnt_child==2){ // ExtDef -> Specifier SEMI;
//             return;
//         }
//         else{
//             assert(root->cnt_child==3);
//             if(strcmp(child_of_no(2, root)->name, "ExtDecList")==0){ // no global variables
//                 return;
//             }
//             else if(strcmp(child_of_no(2, root)->name, "FunDec")==0){
//                 Trans_FunDec(child_of_no(2, root));
//                 Trans_CompSt(child_of_no(3, root));
//             }
//         }
//     }

//     // printf("%s out\n", root->name);

// }

// void Trans_CompSt(struct tree_node *root){
//     if(root->cnt_child==4){
//         Trans_DefList(child_of_no(2, root));
//         Trans_StmtList(child_of_no(3, root));
//     }
//     else{
//         assert(root->cnt_child==3);
//         if(strcmp(child_of_no(2, root)->name, "DefList")==0){
//             Trans_DefList(child_of_no(2, root));
//         }
//         else if(strcmp(child_of_no(2, root)->name, "StmtList")==0){
//             Trans_StmtList(child_of_no(2, root));
//         }
//         else assert(0);
//     }
// }

// void Trans_FunDec(struct tree_node *root){
//     Operand func;
//     func.kind = OP_FUNC;

//     strcpy(func.u.name_func, root->first_child->compos.id);

//     Add_InterCode(CodeList, Gen_Code(FUNC, func, empty, empty, size, rel));

//     if(root->cnt_child==4){
//         Trans_VarList(child_of_no(3, root));
//     }
// }

// void Trans_DefList(struct tree_node *root){
//     Trans_Def(root->first_child);
//     if(root->cnt_child==2) Trans_DefList(child_of_no(2, root));
// }

// void Trans_Def(struct tree_node *root){
//     Trans_DecList(child_of_no(2, root));
// }

// void Trans_DecList(struct tree_node *root){
//     Trans_Dec(root->first_child);
//     if(root->cnt_child==3) Trans_DecList(child_of_no(3, root));
// }

// void Trans_Dec(struct tree_node *root){
//     if(root->cnt_child==3){
//         Operand t1 = new_temp();
//         Trans_Exp(child_of_no(3, root), t1);

//         Symbol sym;
//         strcpy(sym.name, root->first_child->first_child->compos.id);
//         HashTableNode *cur = Stack_find(page_stack, sym);
//         Operand op1;
//         op1.kind = OP_VARI;
//         op1.u.no_vari = cur->symbol.rank;
//         Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, op1, t1, empty, size, rel));
//     }
//     else if(root->cnt_child == 1){
//         // array
//         Trans_VarDec(root->first_child, SC_DEC, 0);
//     }
// }

// void Trans_StmtList(struct tree_node *root){
//     Trans_Stmt(root->first_child);
//     if(root->cnt_child==2) Trans_StmtList(child_of_no(2, root));
// }

// void Trans_Args(struct tree_node *root, listnode **arg_list){
//     Operand t1;
//     if(root->first_child->type->kind==ARRAYY){
//         Symbol sym;
//         strcpy(sym.name, root->first_child->first_child->compos.id);
//         HashTableNode *arr = Stack_find(page_stack, sym);

//         t1.kind = OP_ARRAY;
//         t1.u.no_array = arr->symbol.rank;
//     }
//     else{
//         assert(root->first_child->type->kind==BASEE);
//         t1 = new_temp();
//         Trans_Exp(root->first_child, t1);
//     }
//     listnode *p = (listnode*)malloc(sizeof(listnode));
//     p->para = t1;
//     listnode *temp = *arg_list;
//     *arg_list = p;
//     (*arg_list)->next = temp;

//     if(root->cnt_child==3){
//         Trans_Args(child_of_no(3, root), arg_list);
//     }
// }

// void Trans_VarList(struct tree_node *root){
//     Trans_ParamDec(root->first_child);
//     if(root->cnt_child==3){
//         Trans_VarList(child_of_no(3, root));
//     }
// }

// void Trans_ParamDec(struct tree_node *root){
//     Trans_VarDec(child_of_no(2, root), SC_FUNC, 0);
// }

// void Trans_VarDec(struct tree_node *root, int source, int size){
//     assert(source==SC_DEC || source==SC_FUNC);

//     if(source==SC_FUNC){
//         if(root->cnt_child==1){ // ID
//             Operand t1 = new_temp();
//             Symbol sym;
//             strcpy(sym.name, root->first_child->compos.id);
//             HashTableNode *cur = Stack_find(page_stack, sym);
//             t1.u.no_vari = cur->symbol.rank;

//             Add_InterCode(CodeList, Gen_Code(PARAM, t1, empty, empty, size, rel));
//         }
//         else{
//             //TAG : para is a array
//             struct tree_node *find_id = root;
//             while(strcmp(find_id->name, "ID")!=0) find_id = find_id->first_child;
//             Operand t1 = new_add();
//             Symbol sym;
//             strcpy(sym.name, find_id->compos.id);
//             HashTableNode *arr = Stack_find(page_stack, sym);

//             arr->symbol.rank = t1.u.no_add;
//             Add_InterCode(CodeList, Gen_Code(PARAM, t1, empty, empty, size, rel));
//         }
//     }
//     else{
//         if(root->cnt_child==1){ // Var -> ID
//             if(size==0) return; // normal variable
//             else{
//                 // declare an array
//                 Symbol sym;
//                 strcpy(sym.name, root->first_child->compos.id);
//                 HashTableNode *cur = Stack_find(page_stack, sym);
//                 assert(cur!=NULL);

//                 Operand op;
//                 op.kind = OP_ARRAY;
//                 op.u.no_array = cur->symbol.rank;

//                 Add_InterCode(CodeList, Gen_Code(DEC, op, empty, empty, size*4, rel));
//             }
//         }
//         else{ // VarDec -> VarDec LB INT RB
//             if(size==0) size = 1;
//             size *= child_of_no(3, root)->compos.val_int;
//             Trans_VarDec(root->first_child, source, size);
//         }
//     }
// }

// void Trans_Array(struct tree_node *root, Operand opde){
//     struct tree_node *cur = root;
//     while(strcmp(cur->name, "ID")!=0) cur = cur->first_child;
//     Symbol sym;
//     strcpy(sym.name, cur->compos.id);
//     HashTableNode *arr = Stack_find(page_stack, sym);

//     assert(arr->symbol.kind==VARIABLEE && arr->symbol.prop.sym_type->kind==ARRAYY);

//     Type *dim = arr->symbol.prop.sym_type;
//     intlist *listhead = (intlist*)malloc(sizeof(intlist)); // an empty node
//     listhead->next = NULL;

//     while(dim!=NULL){
//         intlist *p = (intlist*)malloc(sizeof(intlist));
//         p->val = dim->type.array.size;
//         p->next = listhead->next;
//         listhead->next = p;

//         dim = dim->type.array.type_ele;
//     }

//     Operand addr = new_temp();
//     Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, addr, new_const(0), empty, size, rel));
//     intlist *curdim = listhead->next;
//     int sub_size=1;
//     while(curdim->next!=NULL){
//         curdim = curdim->next;
//         // printf("%d %d\n", root->cnt_child, curdim->val);
//         Operand dim_size = new_temp();
//         Trans_Exp(child_of_no(3, root), dim_size);
//         Add_InterCode(CodeList, Gen_Code(MULTI, dim_size, dim_size, new_const(sub_size), size, rel));
//         Add_InterCode(CodeList, Gen_Code(ADD, addr, addr, dim_size, size, rel));

//         sub_size *= curdim->val;
//         root = root->first_child;
//     }

//     assert(strcmp(root->first_child->name, "ID")==0);

//     Add_InterCode(CodeList, Gen_Code(MULTI, addr, addr, new_const(4), size, rel));

//     struct tree_node *check_global = root;
//     while(strcmp(check_global->name, "ExtDef")!=0) check_global = check_global->father;
//     check_global = child_of_no(2, check_global)->first_child;
//     assert(strcmp(check_global->name, "ID")==0);

//     if(strcmp(check_global->compos.id, "main")==0){
//         Operand arr_addr;
//         arr_addr.kind = OP_ARRAY;
//         arr_addr.u.no_array = arr->symbol.rank;
//         Add_InterCode(CodeList, Gen_Code(ADD, addr, arr_addr, addr, size, rel));
//         Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, addr, empty, size, rel));
//     }
//     else{
//         Operand arr_addr;
//         arr_addr.kind = OP_ADD;
//         arr_addr.u.no_add = arr->symbol.rank;
//         Add_InterCode(CodeList, Gen_Code(ADD, addr, arr_addr, addr, size, rel));
//         Add_InterCode(CodeList, Gen_Code(ASSIGN_VAL2VAL, opde, addr, empty, size, rel));
//     }
// }