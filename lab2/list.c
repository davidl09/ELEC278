#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>

// Null for an empty list.
typedef struct list_node *list;

typedef struct list_node {
    int value;
    list tail;
} list_node;

typedef list *iterator;

list make_node(list new_tail, int val) {
    list temp = malloc(sizeof (list_node));
    temp->tail = new_tail;
    temp->value = val;
    return temp;
}



// Inserts a number at a given position in a list.
void insert_at(iterator iter, int num) {
    // Construct a new node to splice into the list.
    struct list_node *new_node = malloc(sizeof(struct list_node));
    new_node->value = num;

    // 'iter' points to the 'tail' slot where we need to insert the node.
    new_node->tail = *iter;
    *iter = new_node;
}

// Inserts a number at a given position in a list.
// Returns false if the iterator is at the end of the list.
bool remove_at(iterator iter) {
    // Make sure we are not at the end.
    if (*iter == NULL)
        return false;

    // 'iter' points to the 'tail' slot where we need to remove the node.
    struct list_node *old_node = *iter;
    *iter = old_node->tail;
    free(old_node);
    return true;
}

// Checks whether the iterator is at the end of the list.
bool at_end(iterator iter) {
    return (*iter)->tail == NULL;
}

// Gets the value at an iterator.
// Only valid if !at_end(iter).
int get(iterator iter) {
    return (*iter)->value;
}

// Advances an iterator to the next position.
// Returns false if the iterator is at the end of the list.
bool next(iterator *iter) {
    if (!iter || !*iter || at_end(*iter))
        return false;

    *iter = &(**iter)->tail;
    return true;
}

void append(iterator i, int val) {
    while (next(&i)); //move i to end of list
    (*i)->tail = malloc(sizeof(list_node));
    assert(*i); //check that memory allocation worked
    (*i)->tail->value = val;
}

// Prints the contents of the list.
void print_list(list nums) {
    printf("%d\n", nums->value);
    if(nums->tail) print_list(nums->tail);
}

// Swaps adjacent items in the list.
void swap_adjacent(list *nums) {
    if(*nums && (*nums)->tail) {
        int temp = (*nums)->value;
        (*nums)->value = (*nums)->tail->value;
        (*nums)->tail->value = temp;
        swap_adjacent(&((*nums)->tail->tail));
    }
}

// Duplicates all items in the list end to end.
void double_list(list *nums) {
    iterator i = nums;
    iterator j = nums;
    while(next(&j)); //move j to point to last element

    list end = *i; //mark end of current list

    do {
        append(j, (*i)->value);
        next(&j);
        next(&i);
    } while (*i != end && next(&i));
}

// Removes adjacent duplicate items in the list.
void remove_adjacent_duplicates(list *nums) {
    iterator i = nums;
    while (*i && (*i)->tail) {
        if((*i)->value == (*i)->tail->value) {
            remove_at(i);
        }
        next(&i);
    }
}

void delete_list(iterator root) {
    list_node *temp = *root;
    while (next(&root)) {
        free(temp);
        temp = *root;
    }
}



int main() {
    // List is initially empty.

    list numbers = NULL;

    int data[] = {1, 2, 3, 3, 4, 5, 6}, size = sizeof (data) / sizeof(int);

    list temp = numbers = make_node(NULL, data[0]);
    for(int *ptr = &data[1]; ptr < &data[size]; ptr++) {
        temp = temp->tail = make_node(NULL, *ptr);
    }

    printf("Original list:\n");
    print_list(numbers);

    printf("Swapping adjacent items:\n");
    swap_adjacent(&numbers);
    print_list(numbers);

    printf("Duplicating all items:\n");
    double_list(&numbers);
    print_list(numbers);

    printf("Removing adjacent duplicates:\n");
    remove_adjacent_duplicates(&numbers);
    print_list(numbers);


    delete_list(&numbers);


    return 0;
}