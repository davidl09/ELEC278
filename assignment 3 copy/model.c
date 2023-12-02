#include "model.h"
#include "interface.h"
#include "stack.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TEXT_LENGTH 30

DEFINE_STACK(double)

// Global variable to store the models
struct Model models[NUM_ROWS][NUM_COLS]; // store up to 100 models


void model_init() {
    // TODO: implement this.

    // Global variable and data types here
    // Represent contents of a cell (text or numbers)


    // Set models
    int i,j;

    for (i = 0; i < NUM_ROWS; ++i) {
        for (j= 0; j < NUM_COLS; ++j) {
            // Initial value indicated empty model
            models[i][j].type = STR;
            models[i][j].modelNumber = 0;
            models[i][j].modelText = malloc(MAX_TEXT_LENGTH);
            // Check if memory was allocated before checking for text
            if (models[i][j].modelText == NULL) {
                // Handle allocation failure
                exit(EXIT_FAILURE);
            }
            // initialize the text to an empty string or some default value
            *(models[i][j].modelText) = '\0';
        }
    }
}

bool is_valid_num(const char *str) {
    while (*str) {
        if (!(isdigit(*str) || *str == '.')) return false;
        ++str;
    }
    return true;
}

char *skip_blank(const char * text) {
    while (*text && isspace(*text)) {
        text++;
    }
    return (char *)text;
}

bool check_formula(char *eqn) {
    eqn = skip_blank(eqn);
    if('=' != eqn[0]) {
        return false;
    }

    for (; *eqn; ++eqn) {
        eqn = skip_blank(eqn);
        if (isalpha(*eqn)) {
            if (islower(*eqn)) {
                *eqn = (char)toupper(*eqn);
            }
        }
        if (isdigit(*eqn) || *eqn == '+' || *eqn == '.') {
            continue;
        } else{
            return false;
        }
    }
    return true;
}


void set_num_val(ROW row, COL col, const char * const text) {
    //sets modelText to text and sets str to str repr of text
    models[row][col].modelNumber = strtod(text, NULL);
    models[row][col].type = NUM;
    strncpy(models[row][col].modelText, text, MAX_TEXT_LENGTH);
}

void set_string_val(ROW row, COL col, const char * const text) {
    models[row][col].modelNumber = 0;
    models[row][col].type = STR;
    strncpy(models[row][col].modelText, text, MAX_TEXT_LENGTH);
}

bool formula(char *eqn, double *temp) {
    *temp = 0;
    stack stack_number = gen_stack();
    int index = 0;
    while (*eqn) {
        if (eqn[0] == '=' || eqn[0] == ' ') {
            ++eqn;
        }
        else if (eqn[0] == '+') {
            index += 1;
            eqn += 1;
        }
        else if (isalpha(*eqn)) {
            if(!isdigit(eqn[1])) {
                return false;
            }
            ROW current_row = strtol(eqn += 1, &eqn, 10) - 1;
            COL current_col = (COL)(eqn[0] - 'A');
            if (current_col * current_row > NUM_COLS * NUM_ROWS || current_col * current_row < 0) {
                return false;
            }
            push(&stack_number, models[current_row][current_col].modelNumber); //fetch numval from cell
        }

        else if (isdigit(*eqn) || *eqn == '.') {
            push(&stack_number, strtod(eqn, &eqn));
        } else {
            delete_stack(&stack_number);
            return false;
        }

        if (stack_number.size != index + 1) {
            delete_stack(&stack_number);
            return false;
        }

        while (stack_number.size > 0) {
            *temp += pop(&stack_number);
        }
    }
    delete_stack(&stack_number);
    return true;
}

void update_cell_value(ROW row, COL col) {
    if (models[row][col].type == EQN) {
        if (formula(models[row][col].modelText, &(models[row][col].modelNumber))) {
            char *numStr = alloca(MAX_TEXT_LENGTH);
            sprintf(numStr, "%lf", models[row][col].modelNumber);
            update_cell_display(row, col, numStr);
        }

    }
}

void set_cell_value(ROW row, COL col, char *text) {

    strncpy(models[row][col].modelText, text, MAX_TEXT_LENGTH);

    if (is_valid_num(text)) {
        set_num_val(row, col, text);
    }
    else {
        set_string_val(row, col, text);
        if (check_formula(text)) {
            if (formula(text, &(models[row][col].modelNumber))) {
                models[row][col].type = EQN;
                sprintf(text, "%lf\n", models[row][col].modelNumber);
            }
        }
    }

    for (ROW i = ROW_1; i < NUM_ROWS; ++i) {
        for (COL j = COL_A; j < NUM_COLS; ++j) {
            if (!(i == row && j == col)) {
                update_cell_value(i, j);
            }
        }
    }
    // Assuming update_cell_display updates the display of the cell
    update_cell_display(row, col, text);
    free(text);
}



void clear_cell(ROW row, COL col) {
    // Free the text if needed
    free(models[row][col].modelText);
    models[row][col].type = NONE;
    models[row][col].modelNumber = 0.0;
    models[row][col].modelText = NULL;

    //models[row][col] = (struct Model){.type = NONE, .modelNumber = 0, .modelText = NULL};
    // This just clears the display without updating any data structure. You will need to change this.
    update_cell_display(row, col, "");
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

char *get_textual_value(ROW row, COL col) {
    struct Model *this = &models[row][col];
    char *result = malloc(MAX_TEXT_LENGTH);
    switch (this->type) {
        case NUM:
        case STR:
        case EQN:
            strncpy(result, this->modelText, MAX_TEXT_LENGTH);
            break;
        case NONE:
            *result = '\0';
            break;
    }
    return result;
}
