// #ifndef INTERCODE
// #define INTERCODE

// #include<stdio.h>
// #include<stdlib.h>
// #include<assert.h>
// #include<string.h>

// struct Operand_def{
//     enum{
//         OP_VARI,
//         OP_ADD,
//         OP_FUNC,
//         OP_ARRAY,
//         OP_LABEL,
//         OP_CONST
//     } kind;

//     union{
//         int no_vari;
//         int no_add;
//         char name_func[20]; // not need to consider deep copy
//         int no_array;
//         int no_label;
//         int val_const;
//     } u;
// };
// typedef struct Operand_def Operand;

// struct InterCode_def{
//     enum{
//         LABEL,
//         FUNC,
//         ASSIGN_VAL2VAL,
//         ADD,
//         SUB,
//         MULTI,
//         DIV,
//         ASSIGN_ADD2VAL,
//         ASSIGN_PNT2VAL,
//         ASSIGN_VAL2PNT,
//         GOTO,
//         GOTO_COND,
//         RETURN,
//         DEC,
//         ARG,
//         CALL,
//         PARAM,
//         READ,
//         WRITE
//     } kind;

//     struct{
//         Operand op1, op2, op3;
//         int size;
//         char rel[3]; 
//     } u;
// };
// typedef struct InterCode_def InterCode;

// struct InterCodeNode_def{
//     InterCode node;
//     struct InterCodeNode_def *prev, *next; 
// };
// typedef struct InterCodeNode_def InterCodeNode;

// void Add_InterCode(InterCodeNode* head, InterCode code);
// // void Print_OP(Operand* op, FILE* f);
// void Print_Code(InterCode code, FILE* f);
// void Print_List(InterCodeNode* head, FILE* f);
// InterCode Gen_Code(int kind, Operand op1, Operand p2, Operand op3, int size, char *rel);

// #endif