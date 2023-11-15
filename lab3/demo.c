#include "demo.h"
#include "stack.h"

bool check_brackets(const char *str) {
    stack_ptr s = stack_new(); //make stack
    char out;

    while(str && *str && *str != '\n') { //check for newline - strings from scanf input may be \n terminated, not \0
        if (*str == '(' || *str == '{' || *str == '[') {
            stack_push(s, *str); //always push in case of a left bracket
        }
        else if (*str == ')' || *str == '}' || *str == ']') { //if closing bracket
            //check top of stack
            if (!stack_pop(s, &out)) {
                stack_free(s);
                return false;
            }  //if the stack is not empty the current right bracket is mismatched - return false
            //else
            if (*str - out != (out == '(' ? 1 : 2))
                stack_push(s, *str); /*
 * if brackets are matched properly subtracting right from left should give 2 if [ or {, or 1 if (  -  see ascii encoding table
 * Note: the value in out comes from the previous stack_pop call
 * thus if a mismatch is detected the new bracket is pushed to the stack, and the stack will not be emty at the end -> return false;
 * could be optimized by simply returning false here
 */
        }
        str++; //iterate over the string
    }
    bool result = !stack_pop(s, &out); //check if stack empty
    stack_free(s);
    return result;
}
