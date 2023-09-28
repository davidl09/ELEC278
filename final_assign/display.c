//
// Created by davidl09 on 9/27/23.
//

#include "display.h"
#include "sheet.h"

const static int line_per_row = 2, char_per_col = 10, line_per_header = 4, char_per_header = 10;

pointer(display) make_display() {
    pointer(display) temp = alloc_n(1, sizeof(pointer(display)));
    if(!temp) handle_alloc_fail();
    temp = init_display(temp, 24, 80);
    return temp;
}
pointer(display) init_display(pointer(display) this, int rows, int columns) {
    assert(this);
    *this = (display){rows, columns,*init_sheet(
            &this->datasheet,
            (rows - line_per_header) / line_per_row,
            (columns - char_per_header) / char_per_col
            )};
    return this;
}

void free_display(pointer(display) this) {
    free_sheet()
}
