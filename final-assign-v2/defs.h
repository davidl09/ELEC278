#ifndef ASSIGNMENT_DEFS_H
#define ASSIGNMENT_DEFS_H

#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#define NUM_ROWS 10
#define NUM_COLS 7

#define MAXLEN 256

#include <stdint.h>

// Rows of the spreadsheet.
// NOTE: enums are 0-based, so the constant 'ROW_1' has the numerical value 0.
typedef enum row_t {
    ROW_1,
    ROW_2,
    ROW_3,
    ROW_4,
    ROW_5,
    ROW_6,
    ROW_7,
    ROW_8,
    ROW_9,
    ROW_10,
} ROW;

typedef enum col_t {
    COL_A,
    COL_B,
    COL_C,
    COL_D,
    COL_E,
    COL_F,
    COL_G,
} COL;

typedef struct ROWCOL {
    ROW row;
    COL col;
} ROWCOL;

typedef enum cell_t {
    NONE,
    STR,
    NUM,
    EQN,
} CELL_TYPE;

typedef struct treeNode treeNode;
//forward treeNode declaration and typedef

typedef struct cell {
    double numval;
    treeNode *expression;
    char *strval;
    CELL_TYPE type;
    bool isValid;
} cell;

typedef enum nodeType {
    VALUE,
    VALUEREF,
    OPERATOR,
    TOK_OPERATOR, //used to indicate validity of precedence field in union in treeNode struct
} nodeType;

typedef double (*nodeFunc)(double, double);

struct treeNode {
    union {
        struct {
            union { //node has either a precedence (Token mode) or two children (tree node mode)
                struct {
                    struct treeNode *left,
                                    *right;
                };
                int precedence;
            };
            nodeFunc func;
        };
        double value;
        double *valueRef; //non-owning, points into sheet
    };
    nodeType type;
};

#endif //ASSIGNMENT_DEFS_H
