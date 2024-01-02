// #include "Hash.h"
// #include<stdio.h>
// #include<stdlib.h>
// #include<assert.h>
// #include<string.h>

// extern int vari_cnt;
// extern int func_cnt;
// extern int array_cnt;
// extern int label_cnt;

// unsigned int Hash_func(char* str){
//     unsigned int val = 0, i;
//     for(char* cur=str; *cur; cur++){
//         val = (val<<2) + (unsigned int)(*cur);
//         if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
//     }
//     return val;
// }

// HashTable* Hash_Init(){
//     HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable)) ;
//     for(int i=0; i<16384; i++){
//         hash_table->table[i] = NULL;
//     }
//     return hash_table;
// }

// void Hash_Add(HashTable* table, Symbol sym){

//     switch(sym.kind){
//         case FUNCTIONN:
//             func_cnt++;
//             sym.rank = func_cnt;
//             break;
//         case VARIABLEE:
//             if(sym.prop.sym_type->kind==BASEE){
//                 vari_cnt++;
//                 sym.rank = vari_cnt;
//             }
//             else if(sym.prop.sym_type->kind==ARRAYY){
//                 array_cnt++;
//                 sym.rank = array_cnt;
//             }
//             // else assert(0);
//             break;
//         // default:
//             // assert(0);
//     }

//     unsigned int val = Hash_func(sym.name);
//     HashTableNode* p = (HashTableNode*)malloc(sizeof(HashTableNode));
//     p->symbol = sym;
//     p->nxt = NULL;

//     HashTableNode* cur = table->table[val];
//     if(cur==NULL) table->table[val] = p;
//     else{
//         assert(strcmp(cur->symbol.name, sym.name)!=0);
//         while(cur->nxt != NULL){
//             assert(strcmp(cur->symbol.name, sym.name)!=0);
//             cur = cur->nxt;
//         }
//         cur->nxt = p;
//     }
// }

// HashTableNode* Hash_Find(HashTable* table, Symbol sym){
//     unsigned int val = Hash_func(sym.name);
//     HashTableNode* cur = table->table[val];
//     while(cur != NULL){
//         if(strcmp(cur->symbol.name, sym.name)==0){
//             return cur;
//         }
//         cur = cur->nxt;
//     }
//     return NULL;
// }
