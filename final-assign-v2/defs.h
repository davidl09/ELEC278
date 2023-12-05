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
typedef enum row_t : uint8_t {
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

typedef enum col_t : uint8_t {
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

typedef enum cell_t : uint8_t {
    NONE,
    STR,
    NUM,
    EQN,
} CELL_TYPE;

typedef struct treeNode treeNode;

typedef struct cell {
    CELL_TYPE type;
    double numval;
    treeNode *expression;
    bool isValid;
    char *strval;
} cell;

typedef enum nodeType : uint8_t {
    VALUE,
    VALUEREF,
    OPERATOR,
    TOK_OPERATOR, //used to indicate validity of precedence field in union in treeNode struct
} nodeType;

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
