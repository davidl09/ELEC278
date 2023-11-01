#ifndef LAB4_ENGINE_H
#define LAB4_ENGINE_H

#include <stdbool.h>

#include "queue.h"

struct context {
    // Queue of elements.
    struct queue q;
    // Temporary variables.
    int x, y;
};

// Represents an error that occurred while parsing or evaluating the input.
struct error {
    // The position in the input string where the error occurred.
    const char *pos;
    // A description of the error; this must be a string literal, not a dynamically allocated string.
    const char *desc;
};

// Runs a sequence of statements, advancing '*input' to the end of the sequence.
// Returns false and sets '*err' if the input was invalid.
bool run_statements(struct context *ctx, const char **input, struct error *err);

// Runs a single statement, advancing '*input' to the end of the statement.
// Returns false and sets '*err' if the input was invalid.
bool run_statement(struct context *ctx, const char **input, struct error *err);

// Evaluates an expression, advancing '*input' to the end of the expression and storing the result in '*out'.
// Returns false and sets '*err' if the input was invalid.
bool eval_expression(struct context *ctx, const char **input, struct error *err, int *out);

// Evaluates a condition, advancing '*input' to the end of the condition and storing the result in '*out'.
// Returns false and sets '*err' if the input was invalid.
bool eval_condition(struct context *ctx, const char **input, struct error *err, bool *out);

// Tries to parse an integer.
bool parse_integer(const char **input, struct error *err, int *out);

#endif //LAB4_ENGINE_H
