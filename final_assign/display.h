//
// Created by davidl09 on 9/27/23.
//

#ifndef ASSIGNMENT_DISPLAY_H
#define ASSIGNMENT_DISPLAY_H

#include <stdlib.h>
#include <assert.h>
#include <curses.h>

#define pointer(thing) thing*

void handle_alloc_fail();
int realloc_n(void **old, int num, int sizeofthing);
void *alloc_n(int num, int sizeofthing);

typedef struct cell {
    int bufsize;
    pointer(char) buffer;
} cell;

void resize_buf_cell(pointer(cell) this, int numthings);
pointer(cell) init_cell(pointer(cell) temp);
pointer(cell) make_cell();
void delete_cell(pointer(cell) this);

typedef struct sheet {
    int rows;
    int columns;
    pointer(cell) cells;
} sheet;

pointer(sheet) make_sheet();
pointer(sheet) init_sheet(pointer(sheet) this, int rows, int columns);

typedef struct display {
    int linecount;
    int charcount;
} display;

#endif //ASSIGNMENT_DISPLAY_H
