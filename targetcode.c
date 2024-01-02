// #include "targetcode.h"

// VariList *VariListHead = NULL;
// int tot_offset = 4;
// int para_cnt = 0;
// int arg_offset = 0;
// char cur_func[50];

// void init_tar(FILE *fout){
//     fprintf(fout, ".data\n");
//     fprintf(fout, "_prompt: .asciiz \"Enter an integer:\"\n");
//     fprintf(fout, "_ret: .asciiz \"\\n\"\n");
//     fprintf(fout, ".globl main\n");
//     fprintf(fout, ".text\n");
//     fprintf(fout, "\n");
//     fprintf(fout, "read:\n");
//     fprintf(fout, "    li $v0, 4\n");
//     fprintf(fout, "    la $a0, _prompt\n");
//     fprintf(fout, "    syscall\n");
//     fprintf(fout, "    li $v0, 5\n");
//     fprintf(fout, "    syscall\n");
//     fprintf(fout, "    la $sp, -8($fp)\n");
//     fprintf(fout, "    move $t8, $ra\n");
//     fprintf(fout, "    lw $ra, -4($fp)\n");
//     fprintf(fout, "    lw $fp, -8($fp)\n");
//     fprintf(fout, "    jr $t8\n");
//     fprintf(fout, "\n");
//     fprintf(fout, "write:\n");
//     fprintf(fout, "    li $v0, 1\n");
//     fprintf(fout, "    syscall\n");
//     fprintf(fout, "    li $v0, 4\n");
//     fprintf(fout, "    la $a0, _ret\n");
//     fprintf(fout, "    syscall\n");
//     fprintf(fout, "    move $v0, $0\n");
//     fprintf(fout, "    la $sp, -12($fp)\n");
//     fprintf(fout, "    move $t8, $ra\n");
//     fprintf(fout, "    lw $ra, -4($fp)\n");
//     fprintf(fout, "    lw $fp, -8($fp)\n");
//     fprintf(fout, "    jr $t8\n");
// }

// void TgtCodeList(InterCodeNode *CodeList, FILE *fout){
//     init_tar(fout);
//     InterCodeNode *code = CodeList->next;
//     while(code!=NULL){
//         if(code->node.kind == FUNC){
//             tot_offset = 4;
//             my_alloc(code);
//         }
//         TgtCode(code->node, fout);
//         code = code->next;
//     }
// }

// void TgtCode(InterCode code, FILE *fout){
//     switch(code.kind){
//         case LABEL:
//             fprintf(fout, "label%d:\n", code.u.op1.u.no_label);
//             break;
//         case FUNC:
//             TgtFunc(code, fout);
//             break;
//         case GOTO:
//             fprintf(fout, "    j label%d\n", code.u.op1.u.no_label);
//             break;
//         case ADD:
//             TgtAdd(code, fout);
//             break;
//         case SUB:
//             TgtSub(code, fout);
//             break;
//         case MULTI:
//             TgtMulti(code, fout);
//             break;
//         case DIV:
//             TgtDiv(code, fout);
//             break;
//         case ASSIGN_VAL2VAL:
//             TgtAssignVal2Val(code, fout);
//             break;
//         case ASSIGN_VAL2PNT:
//             TgtAssignVal2Pnt(code, fout);
//             break;
//         case ASSIGN_PNT2VAL:
//             TgtAssignPnt2Val(code, fout);
//             break;
//         case GOTO_COND:
//             TgtGoCond(code, fout);
//             break;
//         case RETURN:
//             TgtReturn(code, fout);
//             break;
//         case READ:
//             TgtRead(code, fout);
//             break;
//         case WRITE:
//             TgtWrite(code, fout);
//             break;
//         case DEC:
//             TgtDec(code, fout);
//             break;
//         case ARG:
//             TgtArg(code, fout);
//             break;
//         case CALL:
//             TgtCall(code, fout);
//             break;
//         case PARAM:
//             TgtParam(code, fout);
//             break;
//         default:
//             Print_Code(code, fout);
//     }
// }

// void my_alloc(InterCodeNode *code){
//     InterCodeNode *cur = code->next;
//     while(cur!=NULL && cur->node.kind!=FUNC){
//         switch(cur->node.kind){
//             case ADD:
//             case SUB:
//             case MULTI:
//             case DIV:
//                 AddrVar(cur->node.u.op1);
//                 AddrVar(cur->node.u.op2);
//                 AddrVar(cur->node.u.op3);
//                 break;
//             case ASSIGN_VAL2VAL:
//             case ASSIGN_VAL2PNT:
//             case ASSIGN_PNT2VAL:
//                 AddrVar(cur->node.u.op1);
//                 AddrVar(cur->node.u.op2);
//                 break;
//             case GOTO_COND:
//                 AddrVar(cur->node.u.op1);
//                 AddrVar(cur->node.u.op2);
//                 break;
//             case RETURN:
//                 AddrVar(cur->node.u.op1);
//                 break;
//             case READ:
//                 AddrVar(cur->node.u.op1);
//                 break;
//             case WRITE:
//                 AddrVar(cur->node.u.op1);
//                 break;
//             case DEC:
//                 {
//                     VariList *p = (VariList*)malloc(sizeof(VariList));
//                     p->offset = tot_offset;
//                     p->op = cur->node.u.op1;
//                     p->nxt = VariListHead;
//                     VariListHead = p;
//                     tot_offset += cur->node.u.size;
//                 }
//                 break;
//             case ARG:
//                 AddrVar(cur->node.u.op1);
//                 break;
//             case CALL:
//                 AddrVar(cur->node.u.op1);
//                 break;
//             case PARAM:
//                 AddrVar(cur->node.u.op1);
//                 break;
//         }
//         cur = cur->next;
//     }
// }

// void TgtAdd(InterCode code, FILE *fout){
//     if(code.u.op2.kind == OP_CONST){
//         fprintf(fout, "    li $t2, %d\n", code.u.op2.u.val_const);
//     }
//     else if(code.u.op2.kind == OP_ARRAY){
//         fprintf(fout, "    la $t2, %d($fp)\n", AddrVar(code.u.op2));
//     }
//     else{
//         fprintf(fout, "    lw $t2, %d($fp)\n", AddrVar(code.u.op2));
//     }

//     if(code.u.op3.kind == OP_CONST){
//         fprintf(fout, "    addi $t1, $t2, %d\n", code.u.op3.u.val_const);
//     }
//     else{
//         fprintf(fout, "    lw $t3, %d($fp)\n", AddrVar(code.u.op3));
//         fprintf(fout, "    add $t1, $t2, $t3\n");
//     }

//     fprintf(fout, "    sw $t1, %d($fp)\n", AddrVar(code.u.op1));
// }

// void TgtSub(InterCode code, FILE *fout){
//     if(code.u.op2.kind == OP_CONST){
//         fprintf(fout, "    li $t2, %d\n", code.u.op2.u.val_const);
//     }
//     else{
//         fprintf(fout, "    lw $t2, %d($fp)\n", AddrVar(code.u.op2));
//     }

//     if(code.u.op3.kind == OP_CONST){
//         fprintf(fout, "    addi $t1, $t2, -%d\n", -code.u.op3.u.val_const);
//     }
//     else{
//         fprintf(fout, "    lw $t3, %d($fp)\n", AddrVar(code.u.op3));
//         fprintf(fout, "    sub $t1, $t2, $t3\n");
//     }

//     fprintf(fout, "    sw $t1, %d($fp)\n", AddrVar(code.u.op1));
// }

// void TgtMulti(InterCode code, FILE *fout){
//     if(code.u.op2.kind == OP_CONST){
//         fprintf(fout, "    li $t2, %d\n", code.u.op2.u.val_const);
//     }
//     else{
//         fprintf(fout, "    lw $t2, %d($fp)\n", AddrVar(code.u.op2));
//     }

//     if(code.u.op3.kind == OP_CONST){
//         fprintf(fout, "    li $t3, %d\n", code.u.op3.u.val_const);
//     }
//     else{
//         fprintf(fout, "    lw $t3, %d($fp)\n", AddrVar(code.u.op3));
//     }

//     fprintf(fout, "    mul $t1, $t2, $t3\n");
//     fprintf(fout, "    sw $t1, %d($fp)\n", AddrVar(code.u.op1));
// }

// void TgtDiv(InterCode code, FILE *fout){
//     if(code.u.op2.kind == OP_CONST){
//         fprintf(fout, "    li $t2, %d\n", code.u.op2.u.val_const);
//     }
//     else{
//         fprintf(fout, "    lw $t2, %d($fp)\n", AddrVar(code.u.op2));
//     }

//     if(code.u.op3.kind == OP_CONST){
//         fprintf(fout, "    li $t3, %d\n", code.u.op3.u.val_const);
//     }
//     else{
//         fprintf(fout, "    lw $t3, %d($fp)\n", AddrVar(code.u.op3));
//     }

//     fprintf(fout, "    div $t2, $t3\n");
//     fprintf(fout, "    mflo $t1\n");
//     fprintf(fout, "    sw $t1, %d($fp)\n", AddrVar(code.u.op1));
// }

// void TgtAssignVal2Val(InterCode code, FILE *fout){
//     if(code.u.op2.kind==OP_CONST){
//         fprintf(fout, "    li $t1, %d\n", code.u.op2.u.val_const);
//     }
//     else{
//         fprintf(fout, "    lw $t1, %d($fp)\n", AddrVar(code.u.op2));
//     }
//     fprintf(fout, "    sw $t1, %d($fp)\n", AddrVar(code.u.op1));
// }

// void TgtAssignVal2Pnt(InterCode code, FILE *fout){
//     if(code.u.op2.kind==OP_CONST){
//         fprintf(fout, "    li $t1, %d\n", code.u.op2.u.val_const);
//     }
//     else{
//         fprintf(fout, "    lw $t1, %d($fp)\n", AddrVar(code.u.op2));
//     }
//     fprintf(fout, "    lw $t2, %d($fp)\n", AddrVar(code.u.op1));
//     fprintf(fout, "    sw $t1, 0($t2)\n");
// }

// void TgtAssignPnt2Val(InterCode code, FILE *fout){
//     fprintf(fout, "    lw $t1, %d($fp)\n", AddrVar(code.u.op2));
//     fprintf(fout, "    lw $t1, 0($t1)\n");
//     fprintf(fout, "    sw $t1, %d($fp)\n", AddrVar(code.u.op1));
// }

// void TgtGoCond(InterCode code, FILE *fout){
//     char *ins;
//     if(strcmp(code.u.rel, "==")==0) ins = "beq";
//     else if(strcmp(code.u.rel, "!=")==0) ins = "bne";
//     else if(strcmp(code.u.rel, ">")==0) ins = "bgt";
//     else if(strcmp(code.u.rel, "<")==0) ins = "blt";
//     else if(strcmp(code.u.rel, ">=")==0) ins = "bge";
//     else if(strcmp(code.u.rel, "<=")==0) ins = "ble";

//     fprintf(fout, "    lw $t1, %d($fp)\n", AddrVar(code.u.op1));
//     fprintf(fout, "    lw $t2, %d($fp)\n", AddrVar(code.u.op2));

//     fprintf(fout, "    %s $t1, $t2, label%d\n", ins, code.u.op3.u.no_label);
// }

// void TgtReturn(InterCode code, FILE *fout){
//     if(strcmp(cur_func, "main")!=0){
//         fprintf(fout, "    lw $v0, %d($fp)\n", AddrVar(code.u.op1));
//         int sp_offset = -(para_cnt*4)-8;
//         fprintf(fout, "    la $sp, %d($fp)\n", sp_offset); // recover $sp
//         fprintf(fout, "    move $t8, $ra\n"); // save $ra
//         fprintf(fout, "    lw $ra, -4($fp)\n"); // recover $ra
//         fprintf(fout, "    lw $fp, -8($fp)\n"); // recover $fp
//         fprintf(fout, "    jr $t8\n"); // return
//     }
//     else{
//         fprintf(fout, "    lw $v0, %d($fp)\n", AddrVar(code.u.op1));
//         fprintf(fout, "    jr $ra\n");
//     }
// }

// void TgtRead(InterCode code, FILE *fout){
//     // fprintf(fout, "    jal read\n");
//     // fprintf(fout, "    sw $v0, %d($fp)\n", AddrVar(code.u.op1));
//     fprintf(fout, "    sw $fp, 0($sp)\n");
//     fprintf(fout, "    sw $ra, 4($sp)\n");
//     fprintf(fout, "    la $fp, 8($sp)\n");
//     arg_offset = 0;
//     fprintf(fout, "    jal read\n");
//     fprintf(fout, "    sw $v0, %d($fp)\n", AddrVar(code.u.op1));
// }

// void TgtWrite(InterCode code, FILE *fout){
//     // fprintf(fout, "    jal write\n");
//     fprintf(fout, "    lw $a0, %d($fp)\n", AddrVar(code.u.op1));
//     fprintf(fout, "    sw $fp, 0($sp)\n");
//     fprintf(fout, "    sw $ra, 4($sp)\n");
//     fprintf(fout, "    la $fp, 8($sp)\n");
//     arg_offset = 0;
//     fprintf(fout, "    jal write\n");
// }

// void TgtFunc(InterCode code, FILE *fout){
//     fprintf(fout, "\n%s:\n", code.u.op1.u.name_func);
//     strcpy(cur_func, code.u.op1.u.name_func);
//     if(strcmp(cur_func, "main")==0){
//         fprintf(fout, "    move $fp, $sp\n");
//     }
//     fprintf(fout, "    addi $sp, $fp, %d\n", tot_offset);
//     para_cnt = 0;
// }

// void TgtDec(InterCode code, FILE *fout){
//     // add an array to stack
//     VariList *p = (VariList*)malloc(sizeof(VariList));
//     p->offset = tot_offset;
//     p->op = code.u.op1;
//     p->nxt = VariListHead;
//     VariListHead = p;
//     tot_offset += 4 * code.u.size;
// }

// void TgtArg(InterCode code, FILE *fout){
//     fprintf(fout, "    lw $t1, %d($fp)\n", AddrVar(code.u.op1));
//     fprintf(fout, "    sw $t1, %d($sp)\n", arg_offset);
//     arg_offset += 4;
// }

// void TgtCall(InterCode code, FILE *fout){
//     fprintf(fout, "    sw $fp, %d($sp)\n", arg_offset);
//     arg_offset += 4;
//     fprintf(fout, "    sw $ra, %d($sp)\n", arg_offset);
//     arg_offset += 4;
//     fprintf(fout, "    la $fp, %d($sp)\n", arg_offset);
//     arg_offset = 0;
//     fprintf(fout, "    jal %s\n", code.u.op2.u.name_func);
//     fprintf(fout, "    sw $v0, %d($fp)\n", AddrVar(code.u.op1));
// }

// void TgtParam(InterCode code, FILE *fout){
//     para_cnt++;
//     int offset = -(para_cnt*4)-8;
//     DeterAddr(code.u.op1, offset);
// }


// int CmpOP(Operand a, Operand b){
//     if(a.kind != b.kind) return 0;

//     switch(a.kind){
//         case OP_VARI:
//             return (a.u.no_vari==b.u.no_vari)? 1:0;
//             break;
//         case OP_ADD:
//             return (a.u.no_add==b.u.no_add)? 1:0;
//             break;
//         case OP_ARRAY:
//             return (a.u.no_array==b.u.no_array)? 1:0;
//             break;
//         default:
//             assert(0);
//     }
// }

// int AddrVar(Operand op){ // offset of fp
//     if(op.kind == OP_CONST || op.kind == OP_FUNC || op.kind == OP_LABEL) return 0;
//     VariList *cur = VariListHead;
//     while(cur!=NULL){
//         if(CmpOP(op, cur->op)==1)
//             return cur->offset;
//         else
//             cur = cur->nxt;
//     }

//     // first time
//     VariList *p = (VariList*)malloc(sizeof(VariList));
//     p->offset = tot_offset;
//     p->op = op;
//     p->nxt = VariListHead;
//     VariListHead = p;
//     tot_offset += 4;
//     return VariListHead->offset;
// }

// void DeterAddr(Operand op, int offset){
//     VariList *cur = VariListHead;
//     while(cur!=NULL){
//         if(CmpOP(op, cur->op)==1){
//             cur->offset = offset;
//             return;
//         }
//         else cur = cur->nxt;
//     }

//     VariList *p = (VariList*)malloc(sizeof(VariList));
//     p->offset = offset;
//     p->op = op;
//     p->nxt = VariListHead;
//     VariListHead = p;
// }