#include "sheet.h"
#include "display.h"

void handle_alloc_fail() {
#ifdef DEBUG
    fprintf(stderr, "MEMORY ALLOCATION FAILED\n");
#endif
    exit(EXIT_FAILURE);
}

int realloc_n(void **old, int num, int sizeofthing) {
    assert(num > 0 && sizeofthing > 0);
    void *temp = realloc(*old, num * sizeofthing);
    if (!temp) handle_alloc_fail();
    *old = temp;
    return 0;
}

void *alloc_n(int num, int sizeofthing) {
    assert(num > 0 && sizeofthing > 0);
    void *temp = calloc(num, sizeofthing);
    if (!temp) handle_alloc_fail();
    return temp;
}

void resize_buf_cell(pointer(cell) this, int numthings) {
    realloc_n((void **)&(this->buffer), numthings, 1);
}

pointer(cell) init_cell(pointer(cell) temp) {
    assert(temp);
    *temp = ((cell){128, alloc_n(128, sizeof(char))});
    temp->buffer[temp->bufsize - 1] = 0; //set null terminator
    return temp;
}

pointer(cell) make_cell() { //allocates memory in the cell buffer
    pointer(cell) temp = alloc_n(1, sizeof(cell));
    return temp;
}

void free_cell(pointer(cell) this) {
    assert(this);
    assert(this->buffer);
    free(this->buffer);
    this->buffer = NULL;
    free(this);
}

pointer(sheet) make_sheet(int rows, int columns) {
    pointer(sheet) this = alloc_n(1, sizeof(sheet));
    init_sheet(this, rows, columns);
    return this;
}

pointer(sheet) init_sheet(pointer(sheet) this, int rows, int columns) {
    *this = (sheet){rows, columns, alloc_n(rows * columns, sizeof(cell))};
    for (int i = 0; i < rows * columns; i++) {
        init_cell(&(this->cells[i]));
    }
    return this;
}

void free_sheet(pointer(sheet) this) {
    assert(this->cells);
    for(int i = 0; i < this->rows * this->columns; i++) {
        free_cell(&(this->cells[i]));
    }
    this->cells = NULL;
    free(this);
}


int main() {
    pointer(display) display_1 = make_display();
    printf("%d\n", display_1->char_col_count);
}
