#include "stack.h"

Stack* StackInit(){
    Stack* self = (Stack*)malloc(sizeof(Stack));
    self->size_ = 1; // an empty symbol table at the bottom
    self->top_ = (StackNode*)malloc(sizeof(StackNode));
    self->top_->table_ = HashInit();
    self->top_->nxt = self->top_->prv = NULL;
    return self;
}

void StackPush(Stack* self, HashTable* table){
    StackNode* new_p = (StackNode*)malloc(sizeof(StackNode));
    new_p->nxt = NULL;
    new_p->prv = self->top_;
    new_p->table_ = table;

    self->top_->nxt = new_p;
    self->top_ = new_p;
    self->size_++;
}

HashTable* StackTop(Stack* self){
    assert(self->size_ > 0);
    return self->top_->table_;
}

void StackPop(Stack* self){
    assert(self->size_ > 0);
    self->size_--;
    StackNode* cur_top = self->top_;
    self->top_ = self->top_->prv;
    self->top_->nxt = NULL;
    free(cur_top);
    // TODO : memory leak here
}

HashTableNode* StackFind(Stack* self, const Symbol sym){
    if(self->size_ <= 0){
        return NULL;
    }

    StackNode* cur_page = self->top_;
    HashTableNode* result = NULL;
    while(cur_page){
        result = HashFind(cur_page->table_, sym);
        if(result){
            break;
        }
        cur_page = cur_page->prv;
    }
    return result;
}

HashTableNode* StackTopFind(Stack* self, const Symbol sym){
    if(self->size_ <= 0){
        return NULL;
    }
    return HashFind(StackTop(self), sym);
}
