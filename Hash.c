#include "Hash.h"
#include<stdio.h>

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
    HashTableNode* cur = table->table[val];
    while(cur != NULL) cur = cur->nxt;
    HashTableNode* p = (HashTableNode*)malloc(sizeof(HashTableNode));
    p->symbol = sym;
    p->nxt = NULL;
    cur->nxt = p;
}

int Hash_Find(HashTable* table, Symbol sym){
    unsigned int val = Hash_func(sym.name);
    HashTableNode* cur = table->table[val];
    while(cur != NULL){
        if(cur->symbol.name == sym.name){
            if(cur->symbol.kind == VARIABLE && sym.kind == VARIABLE) return 1;
            else if(cur->symbol.kind == VARIABLE && sym.kind == FUNCTION) return 2;
            else if(cur->symbol.kind == FUNCTION && sym.kind == VARIABLE) return 3;
            else if(cur->symbol.kind == FUNCTION && sym.kind == FUNCTION) return 4;
        }
        cur = cur->nxt;
    }
    return -1;
}
