#ifndef TYPE_SYMBOL
#define TYPE_SYMBOL

struct Field_def{
    char* name;
    struct Type_def* type;
    struct Field_def* nxt; 
}; // a field of structure
typedef struct Field_def Field;

struct Function_def{
    struct Type_def* retn;
    int Argc_cnt;
    struct Field_def* Argv;
};
typedef struct Function_def Function;

// define of type
struct Type_def{
    enum { BASIC, ARRAY, STRUCTURE } kind;
    union{
        enum {INTT, FLOATT} basic; // int or float
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
struct  Symbol_def{
    char* name;
    enum {VARIABLE, FUNCTION, STRUCTT} kind;
    union{
        Type* sym_type;
        Function* sym_func;
    }u;
    int rank;
};
typedef struct Symbol_def Symbol; 

int TypeMatch(Type* type_a, Type* type_b);
Field* HasFld(Field* fld, char* name);

#endif