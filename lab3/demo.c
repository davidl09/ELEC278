#include "demo.h"
#include "stack.h"

bool check_brackets(const char *str) {
    stack_ptr s = stack_new();
    char *out = malloc(sizeof(char));

    while(str && *str && *str != '\n') {
        if (*str == '(' || *str == '{' || *str == '[') {
            stack_push(s, *str);
        }
        else if (*str == ')' || *str == '}' || *str == ']') { //if closing bracket
            //check top of stack
            if (!stack_pop(s, out)) return false;
            if (*str - *out != (*out == '(' ? 1 : 2)) stack_push(s, *str);
        }
        str++;
    }
    bool result = !stack_pop(s, out);
    stack_free(s);
    free(out);
    return result;
}
