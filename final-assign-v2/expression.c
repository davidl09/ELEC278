//
// Created by davidl09 on 12/4/23.
//

#include <ctype.h>
#include "expression.h"
#include "defs.h"

DEFINE_STACK(treeNode);

DEFINE_FIFO(treeNode);

double add(double lhs, double rhs) {
    return lhs + rhs;
}

double subtract(double lhs, double rhs) {
    return lhs - rhs;
}

double multiply(double lhs, double rhs) {
    return lhs * rhs;
}

double divide(double lhs, double rhs) {
    return lhs / rhs;
}

int precedence(const char input) {
    switch (input) {
        case '/':
            return 1;
        case '*':
            return 2;
        default:
            return 0;
    }
}

double evaluate(treeNode *node) {
    switch (node->type) {
        case VALUE:
            return node->value;
        case VALUEREF:
            return *(node->valueRef);
        case OPERATOR:
            return node->func(evaluate(node->left), evaluate(node->right));
    }
}

nodeFunc getFunc(char operator) {
    switch (operator) {
        case '+':
            return add;
        case '-':
            return subtract;
        case '*':
            return multiply;
        case '/':
            return divide;
        default:
            return add;
    }
}

bool isOperator(const char input) {
    return input == '+' || input == '-' || input == '*' || input == '/';
}

bool nextToken(const char **input, treeNode *result) {
    //result is a non-owning pointer to a treeNode, where the result is to be written
    //advances input to first character after token -> input must be pointer to non-owning pointer
    //returned token may have
    if (!input || !(*input) || !(**input)) return false; //if input is null or at end of string

    *input = skip_whitespace(*input);
    if (isOperator(**input)) {
        *result = (treeNode){.type = TOK_OPERATOR, .precedence = precedence(**input), .func = getFunc(**input)};
        (*input)++; //advance input
        return true;
    }
    else if (isdigit(**input)) {
        result->value = strtod(*input, (char **)input);
        result->type = VALUE;
        //strtod advances input for us
        return true;
    }
    else if (isalpha(**input)) {
        COL col = (COL)(toupper(**input) - 'A');
        const char *errTemp = *input;
        ++(*input);
        ROW row = (ROW)(strtol(*input, (char **)input, 10)) - 1;
        if (
                col < COL_A || col >= NUM_COLS
                ||
                row < ROW_1 || row >= NUM_ROWS
                ) {
            *input = errTemp;
            return false;
        }
        //strtol advances input for us
        result->type = VALUEREF;
        result->valueRef = &(sheet[row][col].numval);
        return true;
    }
    return false;
}



bool shuntingYard(const char *input, treeNode_stack *outputStack) {
    //outputStack points to an UNINITIALIZED treeNode_stack
    //this is an OWNING reference
    //ownership is release upon successful execution of the function

    if (*input == 0) {
        //if not at end of string, parsing is not complete and an error occurred
        return false;
    }

    fifo_treeNode inputQ = fifo_treeNode_make(strlen(input));
    *outputStack = make_treeNode_stack();
    treeNode_stack operatorStack = make_treeNode_stack();

    treeNode temp;
    //fill input queue
    while (nextToken(&input, &temp)) {
        fifo_treeNode_push_back(&inputQ, temp);
        //using queue to reverse input -> read first token first, as opposed to last
    }

    while (fifo_treeNode_size(&inputQ) > 0) {
        treeNode current = fifo_treeNode_pop_front(&inputQ);
        switch (current.type) {
            case VALUE:
            case VALUEREF:
                treeNode_stack_push(outputStack, current);
                break;
            case TOK_OPERATOR: {
                while (operatorStack.size > 0 && operatorStack.sp->precedence >= current.precedence) {
                    treeNode noTokOp = treeNode_stack_pop(&operatorStack);
                    //the tree node is converted from a 'token' (which knows its precedence) to a full tree node (no children yet)
                    noTokOp.type = OPERATOR;
                    noTokOp.left = noTokOp.right = NULL;
                    treeNode_stack_push(outputStack, noTokOp);
                }
                treeNode_stack_push(&operatorStack, current);
                break;
            }
            case OPERATOR:
                fifo_treeNode_free(&inputQ);
                treeNode_stack_delete(&operatorStack);
                treeNode_stack_delete(outputStack);
                return false;
        }
    }

    while (operatorStack.size > 0) {
        treeNode noTokOp = treeNode_stack_pop(&operatorStack);
        //the tree node is converted from a 'token' (which knows its precedence) to a full tree node (no children yet)
        noTokOp.type = OPERATOR;
        noTokOp.left = noTokOp.right = NULL;
        treeNode_stack_push(outputStack, noTokOp);
    }

    fifo_treeNode_free(&inputQ);
    treeNode_stack_delete(&operatorStack);
    return true;
}

void deleteTreeNode(treeNode *node) {
    if (node->type == OPERATOR) {
        if (node->right)
            deleteTreeNode(node->right);
        if (node->left)
            deleteTreeNode(node->left);
    }
}

bool makeTreeExpr(const char *input, treeNode **result) {
    treeNode_stack outStack = make_treeNode_stack(), inStack;
    if (!shuntingYard(input, &inStack)) {
        treeNode_stack_delete(&inStack);
        return false;
    }

    for (treeNode *current = inStack.base; current < inStack.sp; ++current) {
        //iterate over resulting stack like an array
        switch (current->type) {
            case VALUE:
            case VALUEREF:
                treeNode_stack_push(&outStack, *current);
                break;

            case OPERATOR:
                if (outStack.size < 2) {
                    if (outStack.size == 1) {
                        treeNode temp = treeNode_stack_pop(&outStack);
                        deleteTreeNode(&temp);
                    }
                    treeNode_stack_delete(&outStack);
                    return false;
                }

                current->left = malloc(sizeof(treeNode));
                current->right = malloc(sizeof(treeNode));

                *(current->right) = treeNode_stack_pop(&outStack);
                *(current->left) = treeNode_stack_pop(&outStack);
                treeNode_stack_push(&outStack, *current);
                break;
            case TOK_OPERATOR: {
                if (outStack.size > 0) {
                    treeNode temp = treeNode_stack_pop(&outStack);
                    deleteTreeNode(&temp);
                }
                treeNode_stack_delete(&outStack);
                return false;
            }

        }
    }

    if (outStack.size != 1) {
        if (outStack.size > 1) {
            treeNode temp = treeNode_stack_pop(&outStack);
            deleteTreeNode(&temp);
        }
        treeNode_stack_delete(&outStack);
        return false;
    }
    *result = malloc(sizeof(treeNode));
    **result = treeNode_stack_pop(&outStack);
    treeNode_stack_delete(&outStack);
    return true;
}

bool makeExpression(const char *input, expression *result) {
    *result = (expression) {
            .root = NULL,
            .strVal = strdup(input)
    };
    if (!makeTreeExpr(input, &result->root)) {
        free((void *)result->strVal);
        return false;
    }
    return true;
}

bool evalExpression(const char *expr, double *result) {
    if (!expr) return false;
    expression e;
    if (!makeExpression(expr, &e)) return false;

    *result = evaluate(e.root);
    return true;
}


