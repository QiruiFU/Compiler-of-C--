#ifndef TYPE
#define TYPE

#define INT 0
#define FLOAT 1

typedef struct{
    char* name;
    Type* type_field;
    Field* nxt; 
}Field; // 结构体的一个域

typedef struct{
    int type_base;
}Base; // 普通变量

typedef struct{
    Type* type_ele;
    int size;
 }Array; // 数组

typedef struct{
    Field* filed_ptr;
}Struc; // 结构体

#define BASE 0
#define ARRAY 1
#define STRUCT 2

typedef struct{
    int kind;
    union{
        Base base;
        Array array;
        Struc struc;
    }type;
}Type;

typedef struct{
    Type* retn;
    int Argc_cnt;
    Type* Argv;
}Func;

#define VARIABLE 0
#define FUNCTION 1

typedef struct{
    char* name;
    int kind;
    union{
        Type sym_type;
        Func sym_func;
    }prop;
}Symbol;

#endif