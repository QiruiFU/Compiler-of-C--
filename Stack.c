#include "Stack.h"

Stack* StackInit(){
    Stack* self = (Stack*)malloc(sizeof(Stack));
    self->size = 1; // an empty symbol table at the bottom
    self->top = (StackNode*)malloc(sizeof(StackNode));
    self->top->table = Hash_Init();
    self->top->nxt = self->top->prv = NULL;
    return self;
}

void StackPush(Stack* self, HashTable* table){
    StackNode* new_p = (StackNode*)malloc(sizeof(StackNode));
    new_p->nxt = NULL;
    new_p->prv = self->top;
    new_p->table = table;

    self->top->nxt = new_p;
    self->top = new_p;
    self->size++;
}

HashTable* StackTop(Stack* self){
    assert(self->size > 0);
    return self->top->table;
}

void StackPop(Stack* self){
    assert(self->size > 0);
    self->size--;
    StackNode* cur_top = self->top;
    self->top = self->top->prv;
    self->top->nxt = NULL;
    free(cur_top);
    // TODO : memory leak here
}

HashTableNode* StackFind(Stack* self, const Symbol sym){
    if(self->size <= 0){
        return NULL;
    }

    StackNode* cur_page = self->top;
    HashTableNode* result = StackTop(self);
    while(cur_page){
        result = HashFind(cur_page->table, sym);
        if(result){
            break;
        }
        cur_page = cur_page->prv;
    }
    return result;
}

HashTableNode* StackTopFind(Stack* self, const Symbol sym){
    if(self->size <= 0){
        return NULL;
    }
    return HashFind(StackTop(self), sym);
}
