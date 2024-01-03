#include "Hash.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

// extern int vari_cnt;
// extern int func_cnt;
// extern int array_cnt;
// extern int label_cnt;

unsigned int HashFunc(char* str){
    unsigned int val = 0, i;
    for(char* cur=str; *cur; cur++){
        val = (val<<2) + (unsigned int)(*cur);
        if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val;
}

HashTable* HashInit(){
    HashTable* self = (HashTable*)malloc(sizeof(HashTable)) ;
    for(int i=0; i<16384; i++){
        self->table_[i] = NULL;
    }
    return self;
}

void HashAdd(HashTable* self, Symbol sym){

    // switch(sym.kind){
    //     case FUNCTIONN:
    //         func_cnt++;
    //         sym.rank = func_cnt;
    //         break;
    //     case VARIABLEE:
    //         if(sym.prop.sym_type->kind==BASEE){
    //             vari_cnt++;
    //             sym.rank = vari_cnt;
    //         }
    //         else if(sym.prop.sym_type->kind==ARRAYY){
    //             array_cnt++;
    //             sym.rank = array_cnt;
    //         }
    //         // else assert(0);
    //         break;
    //     // default:
    //         // assert(0);
    // }

    unsigned int val = HashFunc(sym.name);
    HashTableNode* p = (HashTableNode*)malloc(sizeof(HashTableNode));
    p->symbol_ = sym;
    p->nxt = NULL;

    HashTableNode* cur = self->table_[val];
    if(cur==NULL) self->table_[val] = p;
    else{ // add to list directly
        p->nxt = cur;
        self->table_[val] = p;
    }
}

HashTableNode* HashFind(HashTable* self, Symbol sym){
    unsigned int val = HashFunc(sym.name);
    HashTableNode* cur = self->table_[val];
    while(cur != NULL){
        if(strcmp(cur->symbol_.name, sym.name)==0){
            break;
        }
        cur = cur->nxt;
    }
    return cur;
}
