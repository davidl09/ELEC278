#include <string.h>
#include "stack.h"

struct stack {
    // Points to dynamically allocated array of size 'capacity', or is NULL.
    char *data;
    // The capacity of the array.
    size_t capacity;
    // The number of entries currently in the stack.
    size_t length;
};

stack_ptr stack_new() {
    stack_ptr s = malloc(sizeof(struct stack));
    s->data = malloc(100 * sizeof(char)); //default size 100 characters to avoid frequent reallocations -> avoid heap segmentation
    s->capacity = 100; //also copy + reallocate are relatively very expensive
    s->length = 0; //new stack is empty
    return s;
}

void stack_free(stack_ptr s) {
    if (s->data != NULL)
        free(s->data); //free data block
    free(s); //free stack struct
}

void stack_push(stack_ptr s, char c) {
    if(s->capacity <= s->length) { //if the stack is full, double the size of the stack with realloc
        char *temp = realloc(s->data, 2 * s->capacity);
        if(!temp) exit(EXIT_FAILURE); //check realloc failure
        s->data = temp;
        s->capacity *= 2;
    }
    s->data[s->length++] = c; //post-increment stack length and write data
}

bool stack_pop(stack_ptr s, char *out) {
    if(s->length == 0) return false;
    *out = s->data[--s->length]; //decrement stack size and write value to pointer
    return true;
}
