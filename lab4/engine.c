#include "engine.h"
#include "util.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

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

bool handle_enq(const char **input, struct context *ctx, struct error *err) {
    skip_whitespace(input);
    if (!starts_with(*input, "ENQ(")) return handle_error(err, *input + 3, "Expected 'ENQ'");

    int begin_val = contains_at(*input, '(');
    if (begin_val == EOF) return handle_error(err, *input + contains_at(*input, 'Q'), "Expected '('");
    *input += begin_val + 1;
    int enq_val = 0;

    if (isdigit(**input)) {
        parse_integer(input, err, &enq_val);
    } else if (**input == 'x') {
        enq_val = ctx->x;
    } else if (**input == 'y') {
        enq_val = ctx->y;
    } else return handle_error(err, *input, "Unknown variable value");
    *input += contains_at(*input, ';') + 1;

    enqueue(&ctx->q, enq_val);
    return true;
}

bool handle_assignment(const char **input, struct context *ctx, struct error *err) {
    skip_whitespace(input);

    int *assign_to = NULL;
    int assign_from = 0;
    switch (**input) {
        case 'x':
            assign_to = &ctx->x;
            break;
        case 'y':
            assign_to = &ctx->y;
            break;
        default:
            return handle_error(err, *input, "Unable to assign to this value/variable");
    }

    int assignment = contains_at(*input, '=');
    if (assignment == EOF) return handle_error(err, *input, "Expected assignment operator");

    *input += assignment + 1;
    skip_whitespace(input);

    switch (**input) {
        case 'x':
            assign_from = ctx->x;
            break;
        case 'y':
            assign_from = ctx->y;
            break;
        default:
            if (isdigit(**input) || **input == '-') {
                if (parse_integer(input, err, &assign_from)) {
                    *input += contains_at(*input, ';') + 2;
                }
                else return false; //parse int failed, err already initialized
            }
            else return handle_error(err, *input, "Unknown variable");
    }

    *assign_to = assign_from;
    return true;
}

bool run_statement(struct context *ctx, const char **input, struct error *err) {
    skip_whitespace(input);

    int stmnt = contains_at(*input, ';');
    if (stmnt == EOF)
        return handle_error(err, "", "Expected ';'");
    if (starts_with(*input, "ENQ(")) {
        return handle_enq(input, ctx, err);
    }
    if (contains_at(*input, '=') != EOF) {
        return handle_assignment(input, ctx, err);
    }

    return true;
}

bool eval_expression(struct context *ctx, const char **input, struct error *err, int *out) {
    // TODO: Task 2: implement variable, constant, and dequeue.

    // TODO: Task 3: implement add.

    return true;
}

bool eval_condition(struct context *ctx, const char **input, struct error *err, bool *out) {
    // TODO: Task 2: implement empty, equals, less than.

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
