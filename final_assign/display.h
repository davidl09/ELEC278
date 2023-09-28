//
// Created by davidl09 on 9/27/23.
//

#ifndef ASSIGNMENT_DISPLAY_H
#define ASSIGNMENT_DISPLAY_H

#include "sheet.h"

typedef struct display {
    int line_count;
    int char_col_count;
    sheet datasheet;
} display;


pointer(display) make_display();
pointer(display) init_display(pointer(display) this, int rows, int columns);
void free_display(pointer(display) this);




#endif //ASSIGNMENT_DISPLAY_H
