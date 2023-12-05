#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "model.h"
#include "testrunner.h"
#include "tests.h"

void run_tests() {
//    treeNode *root;
//    assert(makeTreeExpr("3+4", &root));
//    assert(evaluate(root) == 3+4);
//    deleteTreeNode(root);
//    while (true) {
//        char expr[100];
//        printf("Enter an expression\n");
//        scanf("%s", expr);
//        if (makeTreeExpr(expr, &root)) {
//            printf("%s = %lf\n", expr, evaluate(root));
//            deleteTreeNode(root);
//        }
//        else printf("Error\n");
//    }

    set_cell_value(ROW_2, COL_A, strdup("1.4"));
    assert_display_text(ROW_2, COL_A, strdup("1.4"));
    set_cell_value(ROW_2, COL_B, strdup("2.9"));
    assert_display_text(ROW_2, COL_B, strdup("2.9"));
    set_cell_value(ROW_2, COL_C, strdup("=A2+B2+0.4"));
    assert_edit_text(ROW_2, COL_C, strdup("=A2+B2+0.4"));
    assert_display_text(ROW_2, COL_C, strdup("4.7"));
    set_cell_value(ROW_2, COL_B, strdup("3.1"));
    assert_display_text(ROW_2, COL_C, strdup("4.9"));
}
