#include "type_func.h"
#include<stdio.h>
#include<assert.h>
#include<string.h>

int TypeMatch(Type* type_a, Type* type_b){
    if(type_a==NULL && type_b==NULL) return 1;
    if(type_a==NULL || type_b==NULL) return 0;

    if(type_a->kind!=type_b->kind){
        return 0;
    }

    if(type_a->kind==BASEE){
        if(type_a->type.base==type_b->type.base) return 1;
        else return 0;
    }
    else if(type_a->kind==ARRAYY){
        return TypeMatch(type_a->type.array.type_ele, type_b->type.array.type_ele);
    }
    else if(type_a->kind==STRUCTT){
        Field *fld_a = type_a->type.struc, *fld_b = type_b->type.struc;
        while(fld_a!=NULL && fld_b!=NULL && TypeMatch(fld_a->type_field, fld_b->type_field)){
            fld_a = fld_a->nxt;
            fld_b = fld_b->nxt;
        }

        if(fld_a==NULL && fld_b==NULL) return 1;
        else return 0;
    }
    else assert(0);
}

Field* HasFld(Field* fld, char* name){
    Field* res = fld;
    while(res==NULL && strcmp(res->name, name)!=0) res = res->nxt;
    return res;
}