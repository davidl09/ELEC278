#include "model.h"
#include "interface.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "stack.h"

DEFINE_STACK(double)
//using macro in stack.h, defines a stack datastructure which can function with any type (avoids the need for generic stack with void * and callbacks)

cell sheet[NUM_ROWS][NUM_COLS] = {0};

char *skip_whitespace(const char * text) {
    while (*text && isspace(*text)) {
        text++;
    }
    return (char *)text;
}

bool is_valid_num(const char *str) {
    str = skip_whitespace(str);
    while (*str) {
        if (!(isdigit(*str) || *str == '.')) return false;
        ++str;
    }
    return true;
}

bool is_valid_formula(char *formula) {
    formula = skip_whitespace(formula);
    if (formula[0] != '=') return false;
    ++formula; //skip '=' sign
    for (; *formula; ++formula) {
        formula = skip_whitespace(formula);
        if (isalpha(*formula)) {
            if (islower(*formula)) {
                *formula = (char)toupper(*formula);
            }
        } else if (isdigit(*formula) || *formula == '.' || *formula == '+') {
            continue;
        } else return false;
    }
    return true;
}

void set_num_val(ROW row, COL col, const char * const text) {
    //sets numval to val and sets str to str repr of val
    sheet[row][col].type = NUM;
    sheet[row][col].numval = strtod(text, NULL);
    strncpy(sheet[row][col].strval, text, MAXLEN);
}

void set_string_val(ROW row, COL col, const char * const text) {
    sheet[row][col].type = STR;
    sheet[row][col].numval = 0;
    strncpy(sheet[row][col].strval, text, MAXLEN);
}

void init_cell(ROW row, COL col) {
    //only place where allocation happens for a cell object (constructor)
    sheet[row][col] = (cell){.type = STR, .numval = 0, .strval = malloc(MAXLEN)};
    *(sheet[row][col].strval) = '\0';
}

void model_init() {
    //iterate over global variable declared above;
    for (ROW row = ROW_1; row < NUM_ROWS; row++) {
        for (COL col = COL_A; col < NUM_COLS; col++) {
            //fill sheet with empty strings
            init_cell(row, col);
        }
    }
}

void model_clear() {
    //iterate over global variable declared above;
    for (ROW row = ROW_1; row < NUM_ROWS; row++) {
        for (COL col = COL_A; col < NUM_COLS; col++) {
            //fill sheet with empty strings
            clear_cell(row, col);
        }
    }
}

bool parse_formula(char *formula, double *dest) {
    *dest = 0;
    if (!is_valid_formula(formula)) return false;
    double_stack numstack = make_double_stack();
    int opcount = 0;
    while (*formula) {
        if (formula[0] == '=' || formula[0] == ' ') {
            ++formula;
        }
        else if (formula[0] == '+') {
            ++opcount;
            ++formula;
        }
        else if (isalpha(*formula)) {
            if (!isdigit(formula[1])) return false;
            COL thisCol = (COL)(formula[0] - 'A');
            ROW thisRow = strtol(++formula, &formula, 10) - 1;
            if (thisCol * thisRow > NUM_COLS * NUM_ROWS || thisCol * thisRow < 0) return false;
            double_stack_push(&numstack, sheet[thisRow][thisCol].numval); //fetch numval from cell
        }
        else if (isdigit(*formula) || *formula == '.') {
            double_stack_push(&numstack, strtod(formula, &formula));
        } else {
            double_stack_delete(&numstack);
            return false;
        }
    }

    if (numstack.size != opcount + 1) {
        double_stack_delete(&numstack);
        return false;
    }

    while (numstack.size > 0) {
        *dest += double_stack_pop(&numstack);
    }

    double_stack_delete(&numstack);
    return true;
}

void update_cell_value(ROW row, COL col) {
    if (sheet[row][col].type == EQN) {
        if (parse_formula(sheet[row][col].strval, &(sheet[row][col].numval))) {
            char *numStr = alloca(MAXLEN);
            sprintf(numStr, "%lf", sheet[row][col].numval);
            update_cell_display(row, col, numStr);
        }

    }
}

void set_cell_value(ROW row, COL col, char *text) {
    if (sheet[row][col].type == NONE || sheet[row][col].strval == NULL) {
        init_cell(row, col);
    }
    if (is_valid_num(text)) {
        set_num_val(row, col, text);
    }
    else {
        set_string_val(row, col, text);
        if (is_valid_formula(text)) {
            if (parse_formula(text, &(sheet[row][col].numval))) {
                sheet[row][col].type = EQN;
                sprintf(text, "%lf\0", sheet[row][col].numval);
            }
            else sprintf(text, "Error");
        }
    }
    for (ROW i = ROW_1; i < NUM_ROWS; ++i) {
        for (COL j = COL_A; j < NUM_COLS; ++j) {
            if (!(i == row && j == col)) {
                update_cell_value(i, j);
            }
        }
    }
    update_cell_display(row, col, text);
    free(text);
}

void clear_cell(ROW row, COL col) {
    //frees mem allocated to string member
    free(sheet[row][col].strval);
    sheet[row][col] = (cell){.type = NONE, .numval = 0.0, .strval = NULL};
    update_cell_display(row, col, "");
}

char *get_textual_value(ROW row, COL col) {
    cell *this = &(sheet[row][col]);
    char *result = calloc(MAXLEN, sizeof(char));
    switch (this->type) {
        case EQN:
        case NUM:
        case STR:
            strncpy(result, this->strval, MAXLEN - 1);
            break;
        case NONE:
            *result = '\0';
            break;
    }
    return result;
}
