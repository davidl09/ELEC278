#include <malloc.h>
#include <stdlib.h>
#include "stack.h"

struct stack_node {
    char value;
    struct stack_node *next;
}; //standard linked-list implementation

struct stack {
    // Null if the stack is empty.
    struct stack_node *head;
};

stack_ptr stack_new() {
    stack_ptr s = malloc(sizeof(struct stack));
    s->head = NULL;
    return s;
} //create empty list

void stack_free(stack_ptr s) {
    char out;
    if(s) {
        while(stack_pop(s, &out));
        free(s);
    }
}   //stack_pop frees popped nodes -> pop until stack empty, then free stack

void stack_push(stack_ptr s, char c) {
    struct stack_node *temp = malloc(sizeof(struct stack_node));
    if(!temp) exit(EXIT_FAILURE); //check malloc failure
    *temp = (struct stack_node){c, s->head}; //insert element at front of list (top of stack)
    s->head = temp; //reset head pointer
}

bool stack_pop(stack_ptr s, char *out) {
    if(!(s && s->head)) return false; //check valid list passed
    *out = s->head->value;
    struct stack_node* temp = s->head;
    s->head = s->head->next; //reset head of list
    free(temp); //free old head
    return true;
}