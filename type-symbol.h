#ifndef TYPE_SYMBOL
#define TYPE_SYMBOL

struct Field_def{
    char* name;
    struct Type_def* type;
    struct Field_def* nxt; 
}; // a field of structure
typedef struct Field_def Field;

struct Function_Def{
    Type* retn;
    int Argc_cnt;
    Field* Argv;
};
typedef struct Function_Def Function;

// define of type
struct Type_def{
    enum { BASIC, ARRAY, STRUCTURE } kind;
    union{
        enum {INT, FLOAT} basic; // int or float
        struct{
            struct Type_def* elem;
            int size;
        }array;
        struct{
            char *struct_name;
            struct Field_def* field;
        }structure;
    }u;
};
typedef struct Type_def Type;

// define of symbol
struct  Symbol_Def{
    char* name;
    enum {VARIABLE, FUNCTION, STRUCT} kind;
    union{
        Type* sym_type;
        Function* sym_func;
    }u;
    // int rank;
};
typedef struct Symbol_Def Symbol; 

int TypeMatch(Type* type_a, Type* type_b);
Field* HasFld(Field* fld, char* name);

#endif