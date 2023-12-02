#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFINE_STACK(type)

// The idea behind the algorithem is a LIFO method to add the values in the cell into the stack
// with their corresponding row/column. Once this is done it will be easier locate their values
// when implementing the addition function.

// Complete definition of 'type' struct
struct type {
    double value;  // Or other fields as needed
};

typedef struct {
    struct type *data;
    double *sp;
    size_t size, capacity;
} stack;

stack gen_stack() {
    // DIFF
    struct type *temp;
    temp = malloc(16 * sizeof(double));
    return (stack){temp, (double *) temp, 0, 16};
};

void build_stack(stack *stack1) {
    struct type *temp;
    temp = malloc(2 * stack1->capacity * sizeof(double));
    if(temp) {
        memcpy(temp, stack1->data, 2 * stack1->capacity * sizeof(double));
        stack1->capacity *= 2;
        free(stack1->data);
        stack1->data = temp;
        stack1->sp = (double *) (stack1->data + stack1->size);

    } else {
        exit(EXIT_FAILURE);
    }
}

void push(stack *stack1, double value) {
    if ((size_t) stack1->sp >= stack1->capacity){
        build_stack(stack1);
    }

    *(stack1->sp)++ = value;
    ++stack1->size;
}

double pop(stack *stack1) {
    if(stack1->size == 0) {
        // Check if popping from empty string
        exit(EXIT_FAILURE);
    }else {
        --stack1->size;
        --stack1->sp;
    }
    return *(stack1->sp);
}

void delete_stack(stack *stack1) {
    free(stack1->data);
    stack1->size = 0;
    stack1->capacity = 0;
}











//
//
//
//
//
//
//struct node {
//    int data;
//    struct node *next;
//};
//
//struct node *temp;
//
//void push(); // Insert element into the stack
//void pop(); // Delete element from the stack
//
//void push() {
//    int data = 0;
//    struct node *pointer = (struct node*)malloc(sizeof(struct node));
//    // Handle Stack overflow
//    if (pointer == NULL) {
//        EXIT_FAILURE;
//    } else {
//        if (temp == NULL) {
//            pointer -> data = data;
//            pointer -> next = NULL;
//            temp = pointer;
//        } else{
//            pointer -> data = data;
//            pointer -> next = NULL;
//            temp = pointer;
//        }
//    }
//
//}
//
//void pop() {
//    int item;
//    struct node *pointer;
//    // Handle stack underflow
//    if (temp == NULL) {
//        EXIT_FAILURE;
//    } else{
//        item = temp -> data;
//        pointer = temp;
//        temp = temp -> next;
//        free(pointer);
//    }
//}
//
//
//// For testing purpose only
//void display_stack() {
//    struct node *pointer;
//    pointer = temp;
//    if (pointer == NULL) {
//        // Handle stack Underflow
//        EXIT_FAILURE;
//    } else{
//        while(pointer!=NULL) {
//            printf("%d", pointer->data);
//            pointer = pointer -> next;
//        }
//    }
//}