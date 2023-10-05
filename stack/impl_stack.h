//
// Created by davidl09 on 10/5/23.
//

#include <stdlib.h>
#include <assert.h>


typedef struct FUNC_NAME(stack) {
    T *base;
    T * sp;
    size_t size;
    size_t capacity;
} FUNC_NAME(stack);

FUNC_NAME(stack) FUNC_NAME(make_stack)(){
    FUNC_NAME(stack) result;
    result.base = calloc(16, sizeof(T));
    result.sp = result.base;
    result.size = 0;
    result.capacity = 32;
    return result;
}

void FUNC_NAME(stack_grow)(FUNC_NAME(stack) *stack) {
    //doubles the stack's capacity
    T *temp = realloc(stack->base, (stack->capacity = 2 * stack->capacity));
    if(temp) {
        stack->base = temp;
        stack->sp = stack->base + stack->size;
    }
    else exit(EXIT_FAILURE);
}

void FUNC_NAME(stack_push)(FUNC_NAME(stack) *stack, T value) {
    if (stack->size >= stack->capacity)
        FUNC_NAME(stack_grow)(stack);
    *(stack->sp)++ = value;
    ++stack->size;
}

T FUNC_NAME(stack_pop)(FUNC_NAME(stack) *stack) {
    assert(stack->size > 0);
    stack->size--;
    stack->sp--;
    return *stack->sp;
}

void FUNC_NAME(stack_delete)(FUNC_NAME(stack) *stack) {
    free(stack->base);
    stack->capacity = stack->size = 0;
}