//
// Created by davidl09 on 12/4/23.
//

#include <ctype.h>
#include <math.h>

#include "expression.h"
#include "defs.h"


DEFINE_STACK(treeNode)

DEFINE_FIFO(treeNode)



/**
 * @brief Adds two double values together.
 *
 * This function takes two double values, lhs and rhs, and returns their sum.
 *
 * @param lhs The left-hand side value of the addition.
 * @param rhs The right-hand side value of the addition.
 *
 * @return The sum of the lhs and rhs values.
 */

double add(double lhs, double rhs) {
    return lhs + rhs;
}

/**
 * @brief Subtract two numbers
 *
 * This function takes two double numbers and subtracts the second number from the first number.
 *
 * @param lhs The first number
 * @param rhs The second number
 * @return double The result of the subtraction
 *
 * @note This function assumes that the numbers are of type double.
 *
 * @attention This function does not perform any error handling or input validation.
 * It is the responsibility of the caller to ensure the inputs are valid.
 *
 * @see add()
 * @see multiply()
 * @see divide()
 */

double subtract(double lhs, double rhs) {
    return lhs - rhs;
}

/**
 * @brief Multiplies two numbers.
 *
 * This function takes two double precision numbers as input and returns their product.
 *
 * @param lhs The left-hand side number.
 * @param rhs The right-hand side number.
 * @return The product of lhs and rhs.
 */

double multiply(double lhs, double rhs) {
    return lhs * rhs;
}

/**
 * @brief Divides two numbers
 *
 * This function takes two double values and returns the result of dividing
 * the first number by the second number.
 *
 * @param lhs The value of the dividend
 * @param rhs The value of the divisor
 * @return The result of dividing lhs by rhs
 *
 * @note An exception will be thrown if rhs is zero
 */

double divide(double lhs, double rhs) {
    return lhs / rhs;
}

/**
 * @brief Determines the precedence of an operator.
 *
 * This function determines the precedence of a given operator. The precedence
 * indicates the order in which operators are evaluated in an expression.
 *
 * @param input The operator character for which to determine the precedence.
 * @return An integer representing the precedence of the operator. Higher
 *         values indicate higher precedence.
 *
 * @note This function assumes that the input operator character is valid and
 *       supported.
 */

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

/**
 * @brief Evaluates the given tree node.
 *
 * This function evaluates the given tree node and returns the result of the evaluation.
 *
 * @param node A pointer to the tree node to be evaluated.
 *
 * @return The result of the evaluation.
 */

double evaluate(treeNode *node) {
    switch (node->type) {
        case VALUEREF:
            return *(node->valueRef);
        case OPERATOR:
            return node->func(evaluate(node->left), evaluate(node->right));
        default:
            return node->value;
    }
}

/**
 * @brief Retrieves the corresponding function for the given operator.
 *
 * This function takes an operator character as input and returns the corresponding function
 * that matches the operator. The supported operators are '+', '-', '*', and '/'.
 *
 * @param operator The operator character for which the function is to be retrieved.
 * @return A function pointer to the corresponding function of the operator.
 * @note If the provided operator is not supported, this function returns NULL.
 */

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
        case '^':
            return pow;
        default:
            return add;
    }
}

bool/**
     * @brief Checks if the given character is an operator.
     *
     * This function takes a character as input and checks if it is an operator.
     * An operator is defined as any of the following characters: +, -, *, /, %, ^.
     *
     * @param input The character to be checked.
     * @return True if the character is an operator, false otherwise.
     */

isOperator(const char input) {
    return input == '+' || input == '-' || input == '*' || input == '/' || input == '^';
}

bool/**
     * @brief Retrieves the next token from the input string and populates the result treeNode.
     *
     * This function parses the input string to extract the next token and stores it as a treeNode object.
     *
     * @param input The input string to parse. This is a pointer to a pointer to a char array.
     *              The pointer is updated to point to the location immediately after the extracted token.
     *
     * @param result A pointer to a treeNode object where the parsed token will be stored.
     *               The treeNode object should be preallocated before calling this function.
     *
     * @note This function assumes that the input string is null-terminated.
     *       If the input string is empty, or there are no more tokens left, this function leaves the result treeNode unchanged.
     *
     * @warning The input string should not be modified during the execution of this function,
     *          as it is passed by reference and its value can be updated.
     *
     * @return Void
     */

nextToken(const char **input, treeNode *result) {
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
        int col = (toupper(**input) - 'A');
        const char *errTemp = *input;
        ++(*input);
        int row = (int)strtol(*input, (char **)input, 10) - 1;
        if (
                col < COL_A || col >= NUM_COLS
                ||
                row < ROW_1 || row >= NUM_ROWS
                ) {
            *input = errTemp;
            return false;
        }
//        if (!(sheet[row][col].type == EQN || sheet[row][col].type == NUM)) {
//            return false;
//        }
        //strtol advances input for us
        result->type = VALUEREF;
        result->valueRef = &(sheet[row][col].numval);
        return true;
    }
    return false;
}



bool/**
     * @brief Implements the Shunting Yard algorithm.
     *
     * The Shunting Yard algorithm converts an infix expression to postfix notation.
     * This function takes an input string and a pointer to a stack of treeNodes.
     * It applies the Shunting Yard algorithm to the input string and updates the output stack with the resulting postfix expression.
     *
     * @param input The input string representing the infix expression.
     * @param outputStack A pointer to the stack where the resulting postfix expression will be stored.
     */

shuntingYard(const char *input, treeNode_stack *outputStack) {
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

/**
  * @brief Deletes a given tree node.
  *
  * This function deletes a tree node by freeing the memory associated with it. The function recursively deletes the child nodes of the given node as well.
  *
  * @param node A pointer to the tree node to be deleted.
  *
  * @return void
  */

void deleteTreeNode(treeNode *node) {
    if (node->type == OPERATOR) {
        if (node->right) {
            deleteTreeNode(node->right);
            free(node->right);
            node->right = NULL;
        }
        if (node->left) {
            deleteTreeNode(node->left);
            free(node->left);
            node->left = NULL;
        }
    }
}

/**
     * @brief Creates a binary expression tree from the given input string.
     *
     * The function constructs a binary expression tree by parsing the input string.
     * The resulting expression tree is stored in the **result parameter.
     * The input string must be a valid infix arithmetic expression where operands and operators
     * are separated by whitespace. The supported operators are +, -, *, and /. Parentheses can also be used
     * to group subexpressions.
     *
     * @param input The infix arithmetic expression string.
     * @param result A pointer to a treeNode pointer where the resulting expression tree will be stored.
     *        The caller is responsible for allocating the necessary memory for the result pointer.
     *
     * @note The function assumes that the input string is properly formatted and does not perform
     *       extensive error checking.
     *
     * @return void
     *
     * @sa destroyTreeExpr
     *
     * Example usage:
     * @code{.c}
     *   const char *input = "( 5 + 3 ) * 2";
     *   treeNode *tree;
     *   makeTreeExpr(input, &tree);
     * @endcode
     */

bool makeTreeExpr(const char *input, treeNode **result) {
    treeNode_stack  outStack = make_treeNode_stack(),
                    inStack;
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