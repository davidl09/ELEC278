#include <malloc.h>
#include <stdlib.h>
#include "stack.h"

struct stack_node {
    char value;
    struct stack_node *next;
};

struct stack {
    // Null if the stack is empty.
    struct stack_node *head;
};

stack_ptr stack_new() {
    stack_ptr s = malloc(sizeof(struct stack));
    s->head = NULL;
    return s;
}

void stack_free(stack_ptr s) {
    char out;
    if(s) {
        while(stack_pop(s, &out));
        free(s);
    }
}

void stack_push(stack_ptr s, char c) {
    struct stack_node *temp = malloc(sizeof(struct stack_node));
    if(!temp) exit(EXIT_FAILURE); //check malloc failure
    *temp = (struct stack_node){c, s->head};
    s->head = temp;
}

bool stack_pop(stack_ptr s, char *out) {
    if(!(s && s->head)) return false;
    *out = s->head->value;
    struct stack_node* temp = s->head;
    s->head = s->head->next;
    free(temp);
    return true;
}