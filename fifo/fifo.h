//
// Created by davidl09 on 10/5/23.
//

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define DEFINE_FIFO(T)\
typedef struct fifo_##T {\
T* base_ptr;\
T* insert_ptr;\
T* remove_ptr;\
size_t capacity;\
}fifo_##T;\
\
fifo_##T fifo_##T##_make(size_t buf_size) {\
    fifo_##T temp;\
    temp.base_ptr = temp.remove_ptr = temp.insert_ptr = malloc(buf_size);\
    temp.capacity = buf_size;\
    return temp;\
}\
\
void fifo_##T##_free(fifo_##T *f) {\
    free(f->base_ptr);\
    (f)->base_ptr = (f)->insert_ptr = (f)->remove_ptr = NULL;\
    f->capacity = 0;\
}\
\
size_t fifo_##T##_size(fifo_##T* f) {\
    if(f->insert_ptr >= f->remove_ptr) {\
        return f->insert_ptr - f->remove_ptr;\
    } else {\
        return f->insert_ptr - f->base_ptr + (f->capacity + f->base_ptr - f->remove_ptr);\
    }\
}\
\
void fifo_##T##_push_back(fifo_##T* f, T value) {\
    if (fifo_##T##_size(f) < f->capacity) {\
        if (f->insert_ptr == f->base_ptr + f->capacity) {\
            f->insert_ptr = f->base_ptr;\
        }\
        *f->insert_ptr++ = value;\
    }\
}\
\
T fifo_##T##_pop_front(fifo_##T* f) {\
    assert(fifo_##T##_size(f) > 0);\
    if(f->remove_ptr >= f->base_ptr + f->capacity) {\
        f->remove_ptr = f->base_ptr;\
    }\
    return *(f->remove_ptr)++;\
}\
