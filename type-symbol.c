#include "type-symbol.h"
#include<stdio.h>
#include<assert.h>
#include<string.h>

int TypeMatch(Type* type_a, Type* type_b){
    if(type_a==NULL && type_b==NULL) return 1;
    if(type_a==NULL || type_b==NULL) return 0;

    if(type_a->kind != type_b->kind){
        return 0;
    }

    if(type_a->kind == BASIC){
        if(type_a->u.basic==type_b->u.basic) return 1;
        else return 0;
    }
    else if(type_a->kind == ARRAY){
        if(type_a->u.array.size != type_b->u.array.size) return 0;
        else return TypeMatch(type_a->u.array.elem, type_b->u.array.elem);
    }
    else if(type_a->kind == STRUCTURE){
        if(type_a->u.structure.struct_name==NULL || type_b->u.structure.struct_name==NULL) return 0;
        else {
            return 1 - strcmp(type_a->u.structure.struct_name, type_b->u.structure.struct_name);
        }
    }
    else assert(0);
}

Field* HasFld(Field* fld, char* name){
    Field* res = fld;
    while(res!=NULL && strcmp(res->name, name)!=0) res = res->nxt;
    return res;
}