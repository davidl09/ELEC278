#include "queue.h"
#include <assert.h>
#include <string.h>

const static int QUEUE_INIT_SIZE = 64;

void init_queue(struct queue *q) {
    q->data = NULL;
    q->capacity = 0;
    q->offset = 0;
    q->length = 0;
}

bool queue_is_valid(struct queue *q) {
    return q && q->capacity >= q->length && q->data;
}

void queue_reset_offset(struct queue *q) {
    //move data to front of queue
    int *temp = malloc(q->length * sizeof(int));
    memcpy(temp, q->data + q->offset, q->length);
    memcpy(q->data, temp, q->length);
    free(temp);
    q->offset = 0;
}

bool queue_resize(struct queue *q, size_t size) {

    if (size < q->length) return false;

    queue_reset_offset(q);

    int *reallocTemp = realloc(q->data, size);
    if (reallocTemp) {
        bool rFree = q->data == reallocTemp;

        q->data = reallocTemp;
        if (!rFree) //avoid freeing temp buffer if realloc didnt move pointer
            free(reallocTemp);

        q->capacity = size;
        return true;
    }
    return false;
}

void clear_queue(struct queue *q) {
    assert(q);
    if (q->data != NULL) {
        free(q->data);
        init_queue(q);
    }
}

void enqueue(struct queue *q, int value) {
    if (!(q && q->data)) { //if queue uninitialized, this function allocates space
        q->data = malloc(QUEUE_INIT_SIZE * sizeof (int));
        q->capacity = QUEUE_INIT_SIZE;
    }

    if (q->offset + q->length >= q->capacity)
        queue_resize(q, q->capacity * 2);

    *(q->data + q->offset + q->length++) = value;
}

bool dequeue(struct queue *q, int *out) {
    if (!queue_empty(q)) {
        *out = *(q->data + q->offset++);
        --q->length;

        if (q->capacity > q->length * 3) {
            queue_resize(q, q->capacity / 2 + 1);
        }
        return true;
    }
    return false;
}

bool queue_empty(struct queue *q) {
    return q->length;
}
