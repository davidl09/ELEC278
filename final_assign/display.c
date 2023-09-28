//
// Created by davidl09 on 9/27/23.
//

#include <unistd.h>

#include "display.h"
#include "sheet.h"

const static int line_per_row = 2, char_per_col = 10, line_per_header = 4, char_per_header = 10;

pointer(display) make_display() {
    pointer(display) temp = alloc_n(1, sizeof(display));
    if(!temp) handle_alloc_fail();
    temp = init_display(temp, 24, 80);
    return temp;
}
pointer(display) init_display(pointer(display) this, int rows, int columns) {
    assert(this);
    *this = (display){rows, columns,make_sheet(
            (rows - line_per_header) / line_per_row,
            (columns - char_per_header) / char_per_col
            )};
    /*
     * INITIALIZE NCURSES
     */
    initscr();
    noecho();
    raw();
    keypad(stdscr, true);
    resizeterm(rows, 2 * columns);
    return this;
}

void free_display(handle(display) this) {
    free_sheet(&(*this)->datasheet);
    (*this)->char_col_count = 0;
    (*this)->line_count = 0;
    free(*this);
    *this = NULL;
    endwin();
}

int main() {
    pointer(display) s1 = make_display();
    printf("%d\n", s1->datasheet->rows);
    sleep(10);
    free_display(&s1);
}
