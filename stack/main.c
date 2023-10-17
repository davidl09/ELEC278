#include <stdio.h>

#include "stack.h"

DEFINE_STACK(char)
int main() {
    char_stack myStack = make_char_stack();

    for(char i = 'a'; i <= 'z'; i++) {
        char_stack_push(&myStack, i);
    }

    while(myStack.size > 0) {
        printf("%c\n", char_stack_pop(&myStack));
    }

    char_stack_delete(&myStack);
}

