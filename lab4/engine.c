#include "engine.h"
#include "util.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define max(a,b) (a > b ? : b)

int contains_at(const char * _s_, char c) {
    const char *s = _s_;
    while(*s && *s != c) {
        ++s;
    }
    return (*s ? (int)(s - _s_) : EOF);
}

bool run_statements(struct context *ctx, const char **input, struct error *err) {
    // Skip the whitespace at the beginning of the input.
    skip_whitespace(input);

    // Check if we are at a valid end of a sequence of statements, in which case, do nothing.
    if (**input == '\0' || **input == '}')
        return true;

    // Otherwise, run a single statement and then the rest of the statements.
    return run_statement(ctx, input, err) && run_statements(ctx, input, err);
}

bool handle_error(struct error *error, const char* pos, const char* desc) {
    error->pos = pos;
    error->desc = desc;
    return false;
}

bool handle_assert(const char **input, struct context *ctx, struct error *err) {
    skip_whitespace(input);
    int condition = contains_at(*input, '(');
    if (condition == EOF) return handle_error(err, *input, "Expected '('");
    *input += condition + 1;
    skip_whitespace(input);//input points at first char of condition
    condition = contains_at(*input, ')');
    if (condition == EOF) return handle_error(err, *input, "Expected matching ')'");
    bool out;
    eval_condition(ctx, input, err, &out);
    *input += contains_at(*input, ';') + 1;
    return out ? true : handle_error(err, *input, "Assertion failed");
}

bool handle_enq(const char **input, struct context *ctx, struct error *err) {
    skip_whitespace(input);
    if (!starts_with(*input, "ENQ(")) return handle_error(err, *input + 3, "Expected 'ENQ'");

    int begin_val = contains_at(*input, '(');
    if (begin_val == EOF) return handle_error(err, *input + contains_at(*input, 'Q'), "Expected '('");
    *input += begin_val + 1;
    skip_whitespace(input);
    int enq_val = 0;

    if (!eval_expression(ctx, input, err, &enq_val)) return false;

    *input += contains_at(*input, ';') + 1;

    enqueue(&ctx->q, enq_val);
    return true;
}

bool handle_assignment(const char **input, struct context *ctx, struct error *err) {
    skip_whitespace(input);

    int *assign_to;
    switch(**input) {
        case 'x':
            assign_to = &ctx->x;
            break;
        case 'y':
            assign_to = &ctx->y;
            break;
        default:
            return handle_error(err, *input, "Cannot assign to temporary value");
    }

    int equals = contains_at(*input, '=');
    if (equals == EOF) return handle_error(err, *input, "Expected '='");
    *input += equals + 1;
    skip_whitespace(input);

    //input now points at rhs of asssignment;
    if (!eval_expression(ctx, input, err, assign_to))
        return false;

    *input += contains_at(*input, ';') + 1;

    return true;
}

bool run_statement(struct context *ctx, const char **input, struct error *err) {
    skip_whitespace(input);

    int stmnt = contains_at(*input, ';');
    if (stmnt == EOF) {
        int errpos = max(contains_at(*input, '\n'), contains_at(*input, '\0'));
        return handle_error(err, *input + errpos + 1, "Expected ';'");
    }
    if (starts_with(*input, "ASSERT")) {
        return handle_assert(input, ctx, err);
    }
    if (starts_with(*input, "ENQ(")) {
        return handle_enq(input, ctx, err);
    }
    if (contains_at(*input, '=') != EOF) {
        return handle_assignment(input, ctx, err);
    }


    return false;
}

bool eval_expression(struct context *ctx, const char **input, struct error *err, int *out) {
    //evaluates expression at **input, leaves *input unchanged
    switch (**input) {
        case 'x':
            *out = ctx->x;
            break;
        case 'y':
            *out = ctx->y;
            break;
        case 'D':
            if (!starts_with(*input, "DEQ"))
                return handle_error(err, *input, "Unknown operation");
            if (dequeue(&ctx->q, out) == false) {
                return handle_error(err, *input, "Cannot pop from empty queue");
            }
            break;
        default:
            if (isdigit(**input) || **input == '-') {
                if (!parse_integer(input, err, out))
                    return false; //parse int failed, err already initialized
            }
            else return handle_error(err, *input, "Unknown variable");
    }

    // TODO: Task 3: implement add.

    return true;
}

bool eval_condition(struct context *ctx, const char **input, struct error *err, bool *out) {
    // TODO: Task 2: implement empty, equals, less than.
    skip_whitespace(input);
    if (starts_with(*input, "EMPTY")) {
        *out = queue_empty(&ctx->q);
        return true;
    }
    if (contains_at(*input, '<') != EOF && contains_at(*input, '<') < contains_at(*input, ')')) {
        //handle less than condition
        int lhs, rhs;
        if (isdigit(**input)) {
            if (!parse_integer(input, err, &lhs))
                return handle_error(err, *input, "Could not parse");
        }
        else if (**input == 'x') {
            lhs = ctx->x;
        }
        else if (**input == 'y') {
            lhs = ctx->y;
        }
        else return handle_error(err, *input, "Unknown variable encountered");

        *input += contains_at(*input, '<') + 1;
        skip_whitespace(input); //input points at rhs condition
        if (isdigit(**input)) {
            if (!parse_integer(input, err, &rhs))
                return handle_error(err, *input, "Could not parse");
        }
        else if (**input == 'x') {
            rhs = ctx->x;
        }
        else if (**input == 'y') {
            rhs = ctx->y;
        }
        else return handle_error(err, *input, "Unknown variable encountered");
        *out = (lhs < rhs);
        return true;
    }
    if (contains_at(*input, '=') != EOF && contains_at(*input, '=') < contains_at(*input, ')')) {
        int lhs, rhs;
        if (!eval_expression(ctx, input, err, &lhs)) return false;
        *input += contains_at(*input, '=') + 1;
        skip_whitespace(input);
        if (!eval_expression(ctx, input, err, &rhs)) return false;
        *out = lhs == rhs;
        return true;
    }

    // TODO: Task 3: implement not.

    return true;
}

bool parse_integer(const char **input, struct error *err, int *out) {
    // Try to parse an integer at the current position, advancing '*input' to after it.
    const char *before = *input;
    int result = (int) strtol(before, (char **) input, 10);

    // If no characters were used, then the input was invalid.
    if (*input == before) {
        err->pos = before;
        err->desc = "expected an integer";
        return false;
    } else {
        *out = result;
        return true;
    }
}
