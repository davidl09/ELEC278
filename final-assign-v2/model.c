#include "model.h"
#include "interface.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "stack.h"
#include "expression.h"

DEFINE_STACK(double)
//using macro in stack.h, creates struct and function definition for a stack data structure holding values of type double



cell sheet[NUM_ROWS][NUM_COLS] = {0};

char *skip_whitespace(const char * text) {
    //move 'text' pointer forward while there is no whitespace character, returning a pointer to the first char which is not a whitespace
    while (*text && isspace(*text)) {
        text++;
    }
    return (char *)text;
}

/**
 * @brief Removes leading and trailing whitespace characters from a given string.
 *
 * This function takes a null-terminated string as input and removes any leading
 * and trailing whitespace characters. Whitespace characters include space, tab,
 * newline, carriage return, vertical tab, and form-feed characters.
 *
 * @param text A null-terminated string from which to remove leading and trailing whitespace.
 * @return A pointer to the resulting string with leading and trailing whitespace removed.
 *         If the input string is NULL, NULL is returned.
 */



bool/**
     * @brief Checks if a given string represents a valid number.
     *
     * This function checks if the given string represents a valid number. A valid number
     * should only contain digits, an optional sign (+ or -) at the beginning, and an
     * optional decimal point.
     *
     * Note: This function does not validate if the number is within a specific range or
     * if it exceeds the capacity of a numeric data type.
     *
     * @param str The string to be checked for a valid number.
     * @return true if the string represents a valid number, false otherwise.
     *
     * @remarks The function does not handle exponent notation (e.g., "1e-05"), binary,
     * octal, or hexadecimal numbers. It only supports decimal numbers.
     */

is_valid_num(const char *str) { //checks if the string can be parsed as a floating-point value
    str = skip_whitespace(str);
    while (*str) {
        if (!(isdigit(*str) || *str == '.')) return false;
        ++str;
    }
    return true;
}

bool maybe_formula(const char *formula) {
    formula = skip_whitespace(formula);
    return *formula == '=';
}

bool/**
     * @brief Checks if a given formula is valid.
     *
     * This function checks if a given formula is valid based on the following criteria:
     *  - The formula must not be empty.
     *  - The formula must contain at least one opening and one closing parenthesis.
     *  - The opening and closing parentheses must be properly nested.
     *
     * @param formula The formula to be checked.
     *
     * @return true if the formula is valid, false otherwise.
     */

is_valid_formula(char *formula) {
    //checks if the string can be parsed as a valid formula
    //autocapitalizes any characters in the formula
    formula = skip_whitespace(formula);
    if (formula[0] != '=') return false;
    ++formula; //skip '=' sign
    for (; *formula; ++formula) {
        formula = skip_whitespace(formula);
        if (isalpha(*formula)) {
            if (islower(*formula)) {
                *formula = (char)toupper(*formula);
            }
            char *temp = formula + 1; //temp pointer to check if row index could be parsed
            long row = strtol(formula + 1, &temp, 10);
            row -= 1; // ROW_1 has value 0
            if (formula + 1 == temp || row < 0 || row >= NUM_ROWS) return false;
        } else if (isdigit(*formula) || *formula == '.' || *formula == '+') {
            continue;
        } else return false;
    }
    return true;
}

/**
 * @brief Sets a numeric value to the specified cell in a spreadsheet.
 *
 * This function sets a numeric value to the cell specified by the given row and column indices.
 * The text parameter contains the numerical value as a string, which is converted to a numeric
 * format and assigned to the cell.
 *
 * @param row The row index of the cell in the spreadsheet.
 * @param col The column index of the cell in the spreadsheet.
 * @param text The text containing the numeric value to be assigned to the cell.
 *
 * @return void
 */

void set_num_val(ROW row, COL col, const char * const text) {
    //sets the cell type to 'NUM' and the parse 'text' as a number, storing the result in the cell
    //sets numval to val and sets str to str repr of val
    sheet[row][col].type = NUM;
    sheet[row][col].numval = strtod(text, NULL);
    strncpy(sheet[row][col].strval, text, MAXLEN);
}

/**
 * @brief Set the string value at the specified cell in a grid.
 *
 * This function is used to set the string value at the specified cell in a grid.
 *
 * @param row The row index of the cell in the grid.
 * @param col The column index of the cell in the grid.
 * @param text The string to be set in the cell.
 *
 * @note The 'row' and 'col' arguments should be within the valid range of row and column indices of the grid.
 * @note The 'text' argument should be a non-null pointer to a null-terminated string.
 * @note This function modifies the internal state of the grid to update the string value at the specified cell.
 */

void set_string_val(ROW row, COL col, const char * const text) {
    sheet[row][col].type = STR;
    sheet[row][col].numval = 0;
    strncpy(sheet[row][col].strval, text, MAXLEN);
}

/**
 * @brief Initializes a cell at the specified row and column.
 *
 * This function initializes a cell at the given row and column with default values.
 * The cell's data, formatting, and other properties can be modified after initialization.
 *
 * @param row The row index of the cell.
 * @param col The column index of the cell.
 */

void init_cell(ROW row, COL col) {
    //only place where allocation happens for a cell object (constructor)
    if (sheet[row][col].type != NONE) return;
    sheet[row][col] = (cell){.type = STR, .numval = 0, .strval = calloc(MAXLEN, sizeof(char))};
}

/**
 * @brief Initializes the model.
 *
 * This function allocates memory to each cell's string, setting the cell's type to 'STR'
 *
 * @details The model_init() function is marked with __attribute constructor, and is called automatically before the program's entry point.
 */

__attribute__((constructor))
void model_init() {
    //iterate over global variable declared above;
    for (ROW row = ROW_1; row < NUM_ROWS; row++) {
        for (COL col = COL_A; col < NUM_COLS; col++) {
            //fill sheet with empty strings
            init_cell(row, col);
        }
    }
}

/**
 * @brief Clears the model data.
 *
 * This function clears any existing data in the model. It should be called
 * when you want to start with a fresh slate or reset the model.
 * It is marked as 'destructor', therefore does not need to be called explicitly upon program exit.
 *
 * @note After calling this function, the model will be empty.
 *
 */

__attribute__((destructor))
void model_clear() {
    //iterate over global variable declared above;
    for (ROW row = ROW_1; row < NUM_ROWS; row++) {
        for (COL col = COL_A; col < NUM_COLS; col++) {
            //fill sheet with empty strings
            clear_cell(row, col);
        }
    }
}

/**
 * @brief Parses a formula and extracts the result.
 *
 * This function takes a formula as input and parses it to extract the result. The formula
 * must be a null-terminated string. The result of the formula will be stored in the 'dest' variable.
 *
 * @param[in] formula The formula to be parsed.
 * @param[out] dest Pointer to the variable where the result will be stored.
 *
 * @retval void
 */

bool parse_formula(char *formula, double *dest) {
    *dest = 0;
    if (!is_valid_formula(formula)) return false; //check validity
    double_stack numstack = make_double_stack(); //defined with macro from stack.h
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
            double_stack_delete(&numstack); //free memory associated with stack
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

/***/

void update_cell_value(ROW row, COL col) {
    if (sheet[row][col].type == EQN && sheet[row][col].isValid) {
        char *numStr = alloca(MAXLEN);
        sheet[row][col].numval = evaluate(sheet[row][col].expression);
        sprintf(numStr, "%lg", sheet[row][col].numval);
        update_cell_display(row, col, numStr);
    }
}

/**

 * @brief Sets the value of a cell in a spreadsheet.

 * @param row The row number of the cell.

 * @param col The column number of the cell.

 * @param text The text to set in the cell.

 * @return None.

 */

void set_cell_value(ROW row, COL col, char *text) {
    cell *this = &(sheet[row][col]);
    if (this->type == NONE || this->strval == NULL) {
        init_cell(row, col);
    }
    if (is_valid_num(text)) {
        set_num_val(row, col, text);
    }
    else {
        set_string_val(row, col, text);
        if (maybe_formula(text)) {
            this->type = EQN;
            this->isValid = makeTreeExpr(text + 1, &(this->expression));
            if (this->isValid) {
                this->numval = evaluate(this->expression);
                sprintf(text, "%lg", this->numval);
            }
            else {
                sprintf(text, "#ERROR");
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
    update_cell_display(row, col, text);
    free(text);
}

/**
 * @brief Clears the cell at the specified row and column.
 *
 * This function clears the content of the cell at the given row and column in a grid.
 *
 * @param row The row number of the cell to be cleared.
 * @param col The column number of the cell to be cleared.
 *
 * @return None
 */

void clear_cell(ROW row, COL col) {
    //frees mem allocated to string member
    if (sheet[row][col].type == NONE) return;
    if (sheet[row][col].type == EQN) {
        deleteTreeNode(sheet[row][col].expression);
    }
    free(sheet[row][col].strval);
    sheet[row][col] = (cell){.type = NONE, .numval = 0.0, .strval = NULL};
    update_cell_display(row, col, "");
}

/**
 * @brief Retrieves the textual value from the given row and column.
 *
 * This function returns the textual value present at the specified row and column
 * of a table or a grid. The row and column should be provided as arguments to this
 * function. If the row and/or column is not valid, an appropriate error message will
 * be returned.
 *
 * @param row The row from which the textual value needs to be retrieved.
 * @param col The column from which the textual value needs to be retrieved.
 *
 * @return The textual value present at the specified row and column.
 *         If the row and/or column is invalid, an error message will be returned.
 */

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
