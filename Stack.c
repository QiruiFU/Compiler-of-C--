#include "Stack.h"

Stack* Stack_init(){
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->size = 1;
    stack->top = (StackNode*)malloc(sizeof(StackNode));
    stack->top->table = Hash_Init();
    stack->top->nxt = stack->top->prv = NULL;
    return stack;
}

void Stack_push(Stack* stack, HashTable* table){
    StackNode* new_p = (StackNode*)malloc(sizeof(StackNode));
    new_p->nxt = NULL;
    new_p->prv = stack->top;
    new_p->table = table;

    stack->top->nxt = new_p;
    stack->top = new_p;
    stack->size++;
}

HashTable* Stack_top(Stack* stack){
    assert(stack->size > 0);
    return stack->top->table;
}

void Stack_pop(Stack* stack){
    assert(stack->size > 0);
    stack->size--;
    StackNode* cur_top = stack->top;
    stack->top = stack->top->prv;
    stack->top->nxt = NULL;
    free(cur_top);
    // TODO : memory leak here
}

HashTableNode* Stack_find(Stack* stack, const Symbol sym){
    if(stack->size <= 0){
        return NULL;
    }

    StackNode* cur_node = stack->top;
    HashTableNode* result = NULL;
    while(cur_node){
        result = Hash_Find(cur_node->table, sym);
        if(result){
            return result;
        }
        cur_node = cur_node->prv;
    }
    return NULL;
}

HashTableNode* Stack_top_find(Stack* stack, const Symbol sym){
    if(stack->size <= 0){
        return NULL;
    }

    HashTableNode* result = Hash_Find(Stack_top(stack), sym);
    return result;
}
