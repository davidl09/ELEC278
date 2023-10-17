//
// Created by davidl09 on 9/11/23.
//

#ifndef FIFO_IMPL_FIFO_H
#define FIFO_IMPL_FIFO_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef struct fifo {
    char* base_ptr;
    char* insert_ptr;
    char* remove_ptr;
    size_t capacity;
}fifo_t;

fifo_t make_fifo(size_t buf_size) {
    fifo_t temp;
    temp.base_ptr = temp.remove_ptr = temp.insert_ptr = malloc(sizeof (buf_size);
    temp.capacity = buf_size;
    return temp;
}

void fifo_free(fifo_t *f) {
    free(f->base_ptr);
    (f)->base_ptr = (f)->insert_ptr = (f)->remove_ptr = NULL;
    f->capacity = 0;
}

size_t fifo_size(fifo_t* f) {
    //needs to be improved
    if(f->insert_ptr >= f->remove_ptr) {
        return f->insert_ptr - f->remove_ptr;
    } else {
        return f->insert_ptr - f->base_ptr + (f->capacity + f->base_ptr - f->remove_ptr);
    }
}

void fifo_push_back(fifo_t* f, char value) {
    if (fifo_size(f) < f->capacity) {
        if (f->insert_ptr == f->base_ptr + f->capacity) {
            f->insert_ptr = f->base_ptr;
        }
        *f->insert_ptr++ = value;
    }
}

char fifo_pop_front(fifo_t* f) {
    assert(fifo_size(f));
    if(f->remove_ptr >= f->base_ptr + f->capacity) {
        f->remove_ptr = f->base_ptr;
    }
    return *(f->remove_ptr)++;
}

void fifo_print(fifo_t* f) { //print elements in order of which was added first
    for (char *ptr = f->remove_ptr; ptr != f->insert_ptr; ptr++) {
        if (ptr == f->base_ptr + f->capacity) ptr = f->base_ptr;
        printf("%c\n", *ptr);
    }
}

#endif //FIFO_IMPL_FIFO_H
