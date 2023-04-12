#include "Hash.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

unsigned int Hash_func(char* str){
    unsigned int val = 0, i;
    for(char* cur=str; *cur; cur++){
        val = (val<<2) + (unsigned int)(*cur);
        if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val;
}

void Hash_Add(HashTable* table, Symbol sym){
    unsigned int val = Hash_func(sym.name);
    HashTableNode* p = (HashTableNode*)malloc(sizeof(HashTableNode));
    p->symbol = sym;
    p->nxt = NULL;

    HashTableNode* cur = table->table[val];
    if(cur==NULL) table->table[val] = p;
    else{
        assert(strcmp(cur->symbol.name, sym.name)!=0);
        while(cur->nxt != NULL){
            assert(strcmp(cur->symbol.name, sym.name)!=0);
            cur = cur->nxt;
        }
        cur->nxt = p;
    }
}

HashTableNode* Hash_Find(HashTable* table, Symbol sym){
    unsigned int val = Hash_func(sym.name);
    HashTableNode* cur = table->table[val];
    while(cur != NULL){
        if(strcmp(cur->symbol.name, sym.name)==0){
            return cur;
        }
        cur = cur->nxt;
    }
    return NULL;
}
