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
    s->data = malloc(100 * sizeof(char));
    s->capacity = 100;
    s->length = 0;
    return s;
}

void stack_free(stack_ptr s) {
    if (s->data != NULL)
        free(s->data);
    free(s);
}

void stack_push(stack_ptr s, char c) {
    if(s->capacity <= s->length) {
        char *temp = realloc(s->data, 2 * s->capacity);
        if(!temp) exit(EXIT_FAILURE);
        s->data = temp;
        s->capacity *= 2;
        /*char *temp = malloc(2 * s->capacity);
        if(!temp) exit(EXIT_FAILURE);
        memcpy(temp, s->data, s->length);
        free(s->data);
        s->data = temp;*/
    }
    *(s->data + s->length++) = c;
}

bool stack_pop(stack_ptr s, char *out) {
    if(s->length == 0) return false;
    *out = *(s->data + --s->length);
    return true;
}
