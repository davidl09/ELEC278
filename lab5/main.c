#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct bst_node *bst;

struct bst_node {
    int value;
    bst left, right;
};

void bst_print_elements(bst tree) {
    if (tree != NULL) {
        bst_print_elements(tree->left);
        printf("%d, ", tree->value);
        bst_print_elements(tree->right);
    }
}

void bst_print(bst tree) {
    printf("{");
    bst_print_elements(tree);
    printf("}\n");
}

bst make_node(int value) { //constructor for struct bst_node
    bst temp = malloc(sizeof (struct bst_node));
    temp->value = value;
    temp->left = temp->right = NULL;
    return temp;
}

bool bst_insert(bst *tree, int value) {
    if (!*tree) {
        *tree = make_node(value);
        return true; //create root node if tree is empty
    }
    if (value == (*tree)->value)
        return false; //return false if value already in tree

    bst *target = (value > (*tree)->value ? &(*tree)->right : &(*tree)->left); //select ascending or descending side for  recursion

    if (!*target) {
        *target = make_node(value); //if target side empty make node and return
        return true;
    }
    else return bst_insert(target, value); //else call recursion

}

void bst_delete_node(bst tree) { //free memory node by node
    if (tree->right) {
        bst_delete_node(tree->right);
        free(tree->right);
    }
    if (tree->left) {
        bst_delete_node(tree->left);
        free(tree->left);
    }
}

void bst_delete(bst tree) {
    bst_delete_node(tree);
    free(tree);
}

void bst_transfer(bst *destination, bst source) {
    if (!source) return;
    bst_insert(destination, source->value);
    //recursively insert all nodes of a tree into another
    bst_transfer(destination, source->right);
    bst_transfer(destination, source->left);
}

bst bst_union(bst tree1, bst tree2) {
    bst result = NULL;
    bst_transfer(&result, tree1); //copy tree1 into destination
    bst_transfer(&result, tree2); //copy tree2 into destination, skipping duplicates (uses bst_insert)
    return result;
}

void bst_transfer_intersection(bst *result, bst tree1, bst tree2) {

    if (tree1->value == tree2->value) {
        bst_insert(result, tree1->value);

        //if the nodes are equal, move down left or right in both trees to avoid 'dead' nodes
        if (tree1->left && tree2->left)
            bst_transfer_intersection(result, tree1->left, tree2->left);
        if (tree1->right && tree2->right) {
            bst_transfer_intersection(result, tree1->right, tree2->right);
        }
    }

    //move in increasing or decreasing direction in either tree depending on whether there is a child node and which current node is bigger
    if (tree1->value > tree2->value) {
        if (tree1->left) //avoid descending into a NULL node (segfault)
            bst_transfer_intersection(result, tree1->left, tree2);
        if (tree2->right)
            bst_transfer_intersection(result, tree1, tree2->right);
    }
    if (tree2->value > tree1->value) {
        if (tree1->right)
            bst_transfer_intersection(result, tree1->right, tree2);
        if (tree2->left)
            bst_transfer_intersection(result, tree1, tree2->left);
    }
}

bst bst_intersection(bst tree1, bst tree2) {
    bst result = NULL;
    bst_transfer_intersection(&result, tree1, tree2);
    return result;
}

int main() {
    // Initialize two trees.
    bst tree1 = NULL, tree2 = NULL;

    assert(bst_insert(&tree1, 1));
    assert(bst_insert(&tree1, 9));
    assert(bst_insert(&tree1, 2));
    assert(bst_insert(&tree1, 7));
    assert(bst_insert(&tree1, 0));
    assert(bst_insert(&tree1, 3));
    assert(bst_insert(&tree1, 13));
    assert(bst_insert(&tree1, 5));
    assert(!bst_insert(&tree1, 3));

    // Should print: {0, 1, 2, 3, 5, 7, 9, 13, }
    bst_print(tree1);

    assert(bst_insert(&tree2, 12));
    assert(bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 1));
    assert(bst_insert(&tree2, 0));
    assert(bst_insert(&tree2, 19));
    assert(bst_insert(&tree2, 7));
    assert(!bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 5));

    // Should print: {0, 1, 5, 7, 12, 13, }
    bst_print(tree2);

    // Should print: {0, 1, 2, 3, 5, 7, 9, 12, 13, 19, }
    bst union_1_2 = bst_union(tree1, tree2);
    bst_print(union_1_2);

    // Should print: {0, 1, 5, 7, 13, }
    bst intersection_1_2 = bst_intersection(tree1, tree2);
    bst_print(intersection_1_2);


    bst_delete(tree1);//free memory
    bst_delete(tree2);
    bst_delete(union_1_2);
    bst_delete(intersection_1_2);
    return 0;
}



