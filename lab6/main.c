#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct heap_element {
    int priority;
    const char *data;
} heap_element;

typedef struct heap {
    heap_element *data;
    size_t capacity, length;
} heap;

// Computes the index of the left child, given the parent index.
size_t left_child_of(size_t index) {
    return 3 * index + 1;
}

size_t middle_child_of(size_t index) {
    return 3 * index + 2;
}

// Computes the index of the right child, given the parent index.
size_t right_child_of(size_t index) {
    return 3 * index + 3;
}

// Computes the index of the parent, given a child index.
size_t parent_of(size_t index) {
    return (index - 1) / 3;
}

// Swap two heap elements.
void heap_swap(heap_element *a, heap_element *b) {
    heap_element temp = *a;
    *a = *b;
    *b = temp;
}

size_t level(size_t index) {
    return (size_t)(log(2.0 * (double)index + 1.0) / log(3));
}

size_t depth(const heap h) {
    return level(h.length - 1);
}

void print_heap(const heap h) {
    // Check if the heap is empty
    if (h.length == 0) {
        printf("Heap is empty.\n");
        return;
    }

    size_t this_level = 0;
    printf("=========\n\n");
    for (int j = 0; j < (int)pow(3, (double)depth(h)) - (int)pow(3, (double)this_level); j++) {printf(" ");}
    for (int i = 0; i < h.length; i++) {
        if (level(i) != this_level) {
            printf("\n");
            this_level = level(i);
            for (int j = 0; j < (int)pow(3, (double)depth(h)) - (int)pow(3, (double)this_level); j++) {printf(" ");}
        }
        printf("%d, ", h.data[i].priority);
    }
    printf("\n\n=========\n\n");
}

// Move the element at the given index up the heap until it is in the correct position.
// All ancestors of the given element must be correctly ordered; descendants of the element are ignored.
void heap_up(heap *h, size_t index) {
    // Loop until we reach the root element.
    while (index > 0) {
        size_t parent = parent_of(index);

        // If the element is correctly ordered with respect to its parent, then it is in the correct position.
        if (h->data[parent].priority <= h->data[index].priority)
            break;

        // If it is incorrectly ordered, swap with its parent then continue.
        heap_swap(&h->data[index], &h->data[parent]);
        index = parent;
    }
}

// Move the element at the given index down the heap until it is in the correct position.
// All descendants of the given element must be correctly ordered; ancestors of the element are ignored.
void heap_down(heap *h, size_t index) {
    while (true) {
        // If the left child does not exist, then neither does the right, so exit.
        const size_t left_child = left_child_of(index);
        if (left_child >= h->length)
            break;

        // If the right child doesn't exist, or if the left child is higher priority, then only look at the left child.
        const size_t middle_child = middle_child_of(index);
        const size_t right_child = right_child_of(index);

/*
 * Four possible cases:
 *      1. No children, break;
 *      2. Only Left child, check if ordered and swap accordingly
 *      3. Left and Middle child, check if ordered with middle then left and swap accordingly
 *      4. Left and Middle and Right, check all three and swap accordingly
 */
        //left child always exists at this point
        const bool hasMiddle = middle_child < h->length; //always true if hasRight is true
        const bool hasRight = right_child < h->length;
        if (hasRight) {assert(hasMiddle);}

        if (hasRight && h->data[right_child].priority < h->data[index].priority) {
            heap_swap(&h->data[right_child], &h->data[index]);
            index = right_child;
        }
        else if (hasMiddle && h->data[middle_child].priority < h->data[index].priority) {
            heap_swap(&h->data[middle_child], &h->data[index]);
            index = middle_child;
        }
        else if (h->data[left_child].priority < h->data[index].priority) {
            heap_swap(&h->data[left_child], &h->data[index]);
            index = left_child;
        }
        else break;
/*
        while (true) {
        // If the left child does not exist, then neither does the right, so exit.
        size_t left_child = left_child_of(index);
        if (left_child >= h->length)
            break;

        // If the right child doesn't exist, or if the left child is higher priority, then only look at the left child.
        size_t right_child = right_child_of(index);
        if (right_child >= h->length || h->data[left_child].priority > h->data[right_child].priority) {
            // If the element is incorrectly ordered with respect to the left child, then swap and continue.
            if (h->data[index].priority < h->data[left_child].priority) {
                heap_swap(&h->data[index], &h->data[left_child]);
                index = left_child;
            } else
                break;
        } else if (h->data[index].priority < h->data[right_child].priority) {
            // Here, the element is incorrectly ordered with respect to the right child, so swap and continue.
            heap_swap(&h->data[index], &h->data[right_child]);
            index = right_child;
        } else
            break;
    }
*/

    }
}

void heap_insert(heap *h, int priority, const char *data) {
    // Allocate or re-allocate as needed.
    if (h->capacity == 0) {
        h->capacity = 1;
        h->data = malloc(sizeof(heap_element));
    } else if (h->length == h->capacity) {
        h->capacity *= 2;
        void *temp = realloc(h->data, h->capacity * sizeof(heap_element));
        if (temp) {
            h->data = temp;
        } else exit(EXIT_FAILURE);
    }

    // Set up new element.
    heap_element el = {
            .priority = priority,
            .data = data,
    };

    // Insert new element at bottom of heap, then heap-up to correctly place it.
    size_t index = h->length++;
    h->data[index] = el;
    heap_up(h, index);

}

bool heap_remove(heap *h, int *priority_out, const char **data_out) {
    // Check if the heap is empty.
    if (h->length == 0)
        return false;
    printf("Before removal: \n");
    print_heap(*h);

    // Extract root element.
    *priority_out = h->data[0].priority;
    *data_out = h->data[0].data;

    // Move last element to root, then heap-down to correctly place it.
    h->data[0] = h->data[--h->length];
    heap_down(h, 0);

    // Shrink allocation if applicable.
    if (h->length == 0) {
        h->capacity = 0;
        free(h->data);
        h->data = NULL;
    } else if (h->length * 2 < h->capacity) {
        h->capacity /= 2;
        void *temp = realloc(h->data, h->capacity * sizeof(heap_element));
        if (temp) {
            h->data = temp;
        } else exit(EXIT_FAILURE);
    }
    printf("After removal: \n");
    print_heap(*h);

    return true;
}

int main() {
    heap h = {
            .data = NULL,
            .capacity = 0,
            .length = 0,
    };

    heap_insert(&h, 7, "this");
    heap_insert(&h, 3, "is");
    heap_insert(&h, 0, "an");
    heap_insert(&h, 9, "example");
    heap_insert(&h, 3, "piece");
    heap_insert(&h, 4, "of");
    heap_insert(&h, 8, "text");
    heap_insert(&h, 8, "to");
    heap_insert(&h, 2, "make");
    heap_insert(&h, 1, "sure");
    heap_insert(&h, 7, "your");
    heap_insert(&h, 5, "code");
    heap_insert(&h, 3, "works");
    print_heap(h);

    for (size_t i = 0; i < h.length; i++)
        printf("%d: %d %s\n", (int) i, h.data[i].priority, h.data[i].data);

    puts("Processing all elements according to priority:");

    int priority;
    const char *data;
    while (heap_remove(&h, &priority, &data))
        printf("%d %s\n", priority, data);

    return 0;
}
