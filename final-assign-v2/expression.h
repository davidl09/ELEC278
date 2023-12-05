//
// Created by davidl09 on 12/4/23.
//

#ifndef ASSIGNMENT_EXPRESSION_H
#define ASSIGNMENT_EXPRESSION_H

#include "fifo.h"
#include "stack.h"
#include "defs.h"
#include "model.h"



double add(double lhs, double rhs);

double subtract(double lhs, double rhs);

double multiply(double lhs, double rhs);

double divide(double lhs, double rhs);

double evaluate(treeNode *node);

int precedence(char input);

inline nodeFunc getFunc(char operator);

inline bool isOperator(char input);

bool nextToken(const char **input, treeNode *result);

bool makeTreeExpr(const char *input, treeNode **result);

void deleteTreeNode(treeNode *node);

bool evalExpression(const char *expression, double *result);


#endif //ASSIGNMENT_EXPRESSION_H
