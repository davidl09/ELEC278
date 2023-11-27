#include "model.h"
#include "interface.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "stack.h"

DEFINE_STACK(char);
DEFINE_STACK(double)
//using macro in stack.h, defines a stack datastructure which can function with any type (avoids the need for generic stack with void * and callbacks)

const static size_t MAXLEN = 100;

cell sheet[NUM_ROWS][NUM_COLS] = {0};

char *skip_whitespace(const char * text) {
    while (*text && isspace(*text)) {
        text++;
    }
    return (char *)text;
}

char *skip_cellnum(const char *text) {
    text = skip_whitespace(text);
    if (isalpha(text[0])) {
        ++text;
        while (*text && isdigit(*text)) {
            ++text;
        }
    }
    return (char *)text;
}

char *skip_num(const char *text) {
    text = skip_whitespace(text);
    while(*text && (isdigit(*text) || *text == '.')) {++text;}
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

    while (*formula) { //parse formula
        formula = skip_whitespace(formula);
        if (isalpha(*formula)) {
            if (!isdigit(formula[1])) {
                return false; //check that col letter is followed by row number
            }
            ROWCOL pos;
            pos.col = formula[0] - 'A';
            pos.row = strtol(&formula[1], NULL, 10);
            if (pos.row * pos.col < 0 || pos.row * pos.col > NUM_ROWS * NUM_COLS) { //if out of bounds
                return false;
            }
            formula = skip_cellnum(formula);
            double_stack_push(&numstack, sheet[pos.row][pos.col].numval);
            //fetch value of specified cell and push onto the stack
            continue;
        }
        else if (isdigit(*formula)) {
            double_stack_push(&numstack, strtod(formula, &formula));
            //strtod updates the formula pointer using the passed handle
            continue;
        }
        else if (formula[0] == '+') {
            ++opcount;
            ++formula;
        }
        else if (formula[0] == '=') {
            ++formula;
        }
        else return false;
    }

    if (numstack.size / 2 != opcount) {
        *dest = 0;
        return false;
    }

    while (numstack.size && opcount) {
        *dest += double_stack_pop(&numstack);
        --opcount;
    }

    return true;
}

void set_cell_value(ROW row, COL col, char *text) {
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
        }
    }
    update_cell_display(row, col, text);
    free(text);
}

void clear_cell(ROW row, COL col) {
    //frees mem allocated to string member
    free(sheet[row][col].strval);
    sheet[row][col] = (cell){.type = NONE, .numval = 0, .strval = NULL};
    // This just clears the display without updating any data structure. You will need to change this.
    update_cell_display(row, col, "");
}

char *get_textual_value(ROW row, COL col) {
    cell *this = &sheet[row][col];
    char *result = malloc(MAXLEN);
    switch (this->type) {
        case EQN:
        case NUM:
        case STR:
            strncpy(result, this->strval, MAXLEN);
            break;
        case NONE:
            *result = '\0';
            break;
    }
    return result;
}
