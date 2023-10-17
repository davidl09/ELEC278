#include <stdio.h>

#include "fifo.h"

DEFINE_FIFO(char)

int main() {
    fifo_char queue = fifo_char_make(30);

    for (char i = 'a'; i <= 'z'; i++) {
        fifo_char_push_back(&queue, i);
    }

    while (fifo_char_size(&queue) > 0) {
        printf("%c\n", fifo_char_pop_front(&queue));
    }

}