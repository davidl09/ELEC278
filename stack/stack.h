//
// Created by davidl09 on 10/5/23.
//
#include <stdlib.h>
#include <stdio.h>

#define DEFINE_STACK(type)                      \
typedef struct {                                \
    type *base;                                 \
    type *sp;                                   \
    size_t size;                                \
    size_t capacity;                            \
} type##_stack;                                 \
type##_stack make_##type##_stack() {            \
type *temp = calloc(16, sizeof(type));          \
return (type##_stack){temp, temp, 0, 16};       \
};                                              \
void type##_stack_grow(type##_stack *stack) {   \
type *temp = realloc                            \
(stack->base,                                   \
(stack->capacity = 2 * stack->capacity));       \
if(temp) {                                      \
stack->base = temp;                             \
stack->sp = stack->base + stack->size;          \
}                                               \
else exit(EXIT_FAILURE);                        \
}                                               \
void type##_stack_push                          \
(type##_stack *stack, type value) {             \
if (stack->size >= stack->capacity)             \
    type##_stack_grow(stack);                   \
*(stack->sp)++ = value;                         \
++stack->size;                                  \
}                                               \
type type##_stack_pop(type##_stack *stack){     \
if(stack->size == 0) {                          \
fprintf(stderr, "Error: "                       \
"Tried popping from empty stack");              \
exit(EXIT_FAILURE);                             \
}                                               \
--stack->size;                                  \
--stack->sp;                                    \
return *(stack->sp);                            \
}                                               \
void type##_stack_delete(type##_stack *stack) { \
    free(stack->base);                          \
    stack->size = 0;                            \
    stack->capacity = 0;                        \
}