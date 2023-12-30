#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct avlNode avlNode;

struct avlNode {
    avlNode *left, *right;
    int value, balanceFactor;
};

__attribute__((const))
int max(int a, int b) {
    return a > b ? a : b;
}

__attribute__((const))
int abs(int a) {
    return a < 0 ? -a : a;
}

int heightOf(avlNode *node, int height) {
    if (!node) return height;
    return max(heightOf(node->left, height + 1), heightOf(node->right, height + 1));
}

int getBalance(avlNode *node) {
    if (!node) return 0;
    return heightOf(node->left, 0) - heightOf(node->right, 0);
}

void freeNode(avlNode *node) {
    if (node) {
        freeNode(node->right);
        freeNode(node->left);
        free(node);
    }
}

avlNode *rightRotate(avlNode *node) {
    puts("Right Rotate");
    avlNode *y = node->left;
    avlNode *t2 = y->right;
    y->right = node;
    node->left = t2;
    node->balanceFactor = getBalance(node);
    y->balanceFactor = getBalance(y);
    return y;
}

avlNode *leftRotate(avlNode *node) {
    puts("Left Rotate");
    avlNode *y = node->right;
    avlNode *t2 = y->left;
    y->left = node;
    node->right = t2;
    node->balanceFactor = getBalance(node);
    y->balanceFactor = getBalance(y);
    return y;
}


avlNode *makeAvlNode(int value) {
    avlNode *result = malloc(sizeof (avlNode));
    *result = (avlNode){NULL, NULL, value, 0};
    return result;
}

avlNode *insertAvl__internal__(avlNode *node, int value) {
    if (!node) return makeAvlNode(value);
    if (value < node->value) {
        if (node->left)
            node->left = insertAvl__internal__(node->left, value);
        else {
            node->left = makeAvlNode(value);
        }
    }
    else if (value > node->value) {
        if (node->right)
            node->left = insertAvl__internal__(node->right, value);
        else {
            node->right = makeAvlNode(value);
        }
    }

    if (getBalance(node) < -1) {
        if (getBalance(node->right) == 1) {
            node->right = rightRotate(node->right);
        }
        node = rightRotate(node);
    }
    else if (getBalance(node) > 1) {
        if (getBalance(node->left) == -1) {
            node->left = leftRotate(node->left);
        }
        node = leftRotate(node);
    }
    return node;
}

void insertAvl(avlNode **root, int value) {
    if (!root) return;
    if (!*root) {
        *root = makeAvlNode(value);
        return;
    }
    insertAvl__internal__(*root, value);
    int balance = getBalance(*root);
    if (balance < -1) {
        *root = leftRotate(*root);
    }
    else if (balance > 1) {
        *root = rightRotate(*root);
    }
}

bool avlContains(avlNode *node, int value) {
    if (!node) return false;
    if (node->value == value) {
        return true;
    }
    return avlContains(node->left, value) || avlContains(node->right, value);
}

void printNode(avlNode *node) {
    printf("Balance: %d, Depth: %d, Value: %d\n", getBalance(node), heightOf(node, 0), node->value);
}

void printTree(struct avlNode *node, int indent) {
    if (node != NULL) {
        printTree(node->right, indent + 6);
        printf("%*s%d(%d)\n", indent, "", node->value, getBalance(node));
        printTree(node->left, indent + 6);
    }
}

int main() {
    avlNode *tree;
    int nums[] = {75, 50, 84, 80, 85, 90};
    for (int i = 0; i < sizeof nums / sizeof nums[0]; ++i) {
        insertAvl(&tree, nums[i]);
    }
    printTree(tree, 0);
    insertAvl(&tree, 86);
    printTree(tree, 0);
    freeNode(tree);
}
