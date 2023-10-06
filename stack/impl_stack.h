//
// Created by davidl09 on 10/5/23.
//

#include <stdlib.h>
#include <assert.h>
#include <string.h>


typedef struct {                                
    int *base;
    int *sp;                                   
    size_t size;                                
    size_t capacity;                            
} int_stack;    

int_stack make_int_stack() {            
    int *temp = malloc(16 * sizeof(int));
    return (int_stack){temp, temp, 0, 16};       
};

void int_stack_grow(int_stack *stack) {
    int *temp = malloc(sizeof(int) * 2 * stack->capacity);
    if(temp) {
        memcpy(temp, stack->base, stack->capacity * sizeof(int));
        free(stack->base);
        stack->base = temp;
        stack->sp = stack->base + stack->size;
        stack->capacity *= 2;
    }
    else exit(EXIT_FAILURE);
}

void int_stack_push(int_stack *stack, int value) {
    if (stack->size >= stack->capacity)
        int_stack_grow(stack);
    *(stack->sp) = value;
    stack->sp++;
    ++stack->size;
}                                               
int int_stack_pop(int_stack *stack){     
    if(stack->size == 0) {
        fprintf(stderr, "Error: "
        "Tried popping from empty stack");
        exit(EXIT_FAILURE);
    }
    --stack->size;
    --stack->sp;
    return *(stack->sp);
}                                               
void int_stack_delete(int_stack *stack) { 
    free(stack->base);                          
    stack->size = 0;                            
    stack->capacity = 0;                        
}