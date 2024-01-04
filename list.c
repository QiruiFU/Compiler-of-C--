#include "list.h"

OpList* OpListInit(){
    OpList* self = (OpList*)malloc(sizeof(OpList));
    self->head_ = self->tail_ = NULL;
    return self;
}

void OpListInsert(OpList* self, Operand val){
    OpListNode* p = (OpListNode*)malloc(sizeof(OpListNode));
    p->val_ = val;
    p->prev = NULL;

    if(self->head_ == NULL){
        self->head_ = self->tail_ = p;
        p->next = NULL;
    }
    else{
        p->next = self->head_;
        self->head_->prev = p;
        self->head_ = p;
    }
}

IntList* IntListInit(){
    IntList* self = (IntList*)malloc(sizeof(IntList));
    self->head_ = self->tail_ = NULL;
    return self;
}

void IntListInsert(IntList* self, int val){
    IntListNode* p = (IntListNode*)malloc(sizeof(IntListNode));
    p->val_ = val;
    p->prev = NULL;

    if(self->head_ == NULL){
        self->head_ = self->tail_ = p;
        p->next = NULL;
    }
    else{
        p->next = self->head_;
        self->head_->prev = p;
        self->head_ = p;
    }
}