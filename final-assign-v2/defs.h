#ifndef ASSIGNMENT_DEFS_H
#define ASSIGNMENT_DEFS_H

#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#define NUM_ROWS 10
#define NUM_COLS 7

#define MAXLEN 256

// Rows of the spreadsheet.
// NOTE: enums are 0-based, so the constant 'ROW_1' has the numerical value 0.
typedef enum : int {
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

typedef enum : int {
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

typedef enum {
    NONE,
    STR,
    NUM,
    EQN,
} CELL_TYPE;

typedef struct treeNode treeNode;

typedef struct cell {
    CELL_TYPE type;
    double numval;
    struct {
        treeNode *expression;
        bool isValid;
    };
    char *strval;
} cell;

typedef enum nodeType : unsigned char {
    VALUE,
    VALUEREF,
    OPERATOR,
    TOK_OPERATOR, //used to indicate validity of precedence field in union in treeNode struct
} nodeType;

typedef enum operator : unsigned char {
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
} operatorType;

typedef double (*nodeFunc)(double, double);

typedef struct treeNode {
    union {
        struct {
            union {
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
} treeNode;

typedef struct expression {
    treeNode *root;
    const char *strVal;
} expression;

#endif //ASSIGNMENT_DEFS_H
