// #ifndef TYPE_FUNC
// #define TYPE_FUNC

// #define BASEE 0
// #define ARRAYY 1
// #define STRUCTT 2

// #define INTT 0
// #define FLOATT 1

// struct Type_def{
//     int kind;
//     union{
//         int base; // int or float
//         struct{
//             struct Type_def* type_ele;
//             int size;
//         }array;
//         struct Field_def* struc;
//     }type;
//     char *struct_name;
// };
// typedef struct Type_def Type;

// struct Field_def{
//     char name[40];
//     struct Type_def* type_field;
//     struct Field_def* nxt; 
// }; // 结构体的一个域
// typedef struct Field_def Field;

// typedef struct{
//     Type* retn;
//     int Argc_cnt;
//     Field* Argv;
// }Func;

// #define VARIABLEE 0
// #define FUNCTIONN 1
// #define STRUCTT 2

// typedef struct{
//     char name[40];
//     int kind;
//     union{
//         Type* sym_type;
//         Func* sym_func;
//     }prop;
//     int rank;
// }Symbol;

// int TypeMatch(Type* type_a, Type* type_b);
// Field* HasFld(Field* fld, char* name);

// #endif