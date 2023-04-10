#ifndef TYPE_FUNC
#define TYPE_FUNC

#define BASEE 0
#define ARRAYY 1
#define STRUCTT 2

#define INTT 0
#define FLOATT 1

struct Type_def{
    int kind;
    union{
        int base; // int or float
        struct{
            struct Type_def* type_ele;
            int size;
        }array;
        struct{
            char* name;
            struct Type_def* type_field;
            struct Field_def* nxt; 
        }struc;
    }type;
};
typedef struct Type_def Type;

struct Field_def{
    char* name;
    struct Type_def* type_field;
    struct Field_def* nxt; 
}; // 结构体的一个域
typedef struct Field_def Field;

typedef struct{
    Type retn;
    int Argc_cnt;
    Type* Argv;
}Func;

#define VARIABLEE 0
#define FUNCTIONN 1

typedef struct{
    char name[10];
    int kind;
    union{
        Type sym_type;
        Func sym_func;
    }prop;
}Symbol;

#endif