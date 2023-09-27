#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

typedef struct Node Node;

#pragma pack(4)
struct Node {
    int val;
    Node *next;
};
#pragma pop

Node *make_node(int val) {
    Node *root = malloc(sizeof (Node));
    root->val = val;
    root->next = NULL;
    return root;
}

Node *make_node_ptr(Node *new_next, int val) {
    Node *root = malloc(sizeof (Node));
    root->val = val;
    root->next = new_next;
    return root;
}

bool next(Node **root) {
    if(root && *root && (*root)->next) {
        *root = (*root)->next;
        return true;
    }
    return false;
}

void free_to_tail (Node **root) { //delete all nodes from Node *root to tail of list
    //pointer to pointer is passed to ensure root node ptr is set to NULL
    Node* temp = *root;
    while(*root){
        temp = *root;
        *root = (*root)->next;
        free(temp);
        temp = NULL;
    }

}

void append(Node *root, int val) {
    while (root->next && (root = root->next));
    root->next = make_node(val);
}

void prepend(Node **root, int val) {
    *root = make_node_ptr(*root, val);
}

void insert_node(Node* root, int nth_from_0, int val) {
    if(!nth_from_0) {
        prepend(&root, val);
        return;
    }
    for (; root != NULL && --nth_from_0; root = root->next);
    if (nth_from_0 > 0) return;
    root->next = make_node_ptr(root->next, val);
}

void remove_node(Node *root, int nth_from_0) {
    assert(nth_from_0 > 0);
    for (; root != NULL && --nth_from_0; root = root->next);
    if (nth_from_0 > 0) return;
    Node* temp = root->next;
    root->next = root->next->next;
    free(temp);
}

size_t list_size(Node* root) {
    size_t i = (root == NULL ? 0 : 1);
    while(root && (root = root->next)) {i++;}
    return i;
}

void swap_adjacent(Node *root) {
    do {
        if(!(root && root->next)) return;
        //XOR swap
        root->next->val ^= root->val;
        root->val ^= root->next->val;
        root->next->val ^= root->val;
    } while (next(&root) && next(&root));
}

void double_list(Node *root) {
    Node *end = root;

    while(next(&end)); //move iterator to end of list

    int size = (int)list_size(root);
    for (int i = 0; i < size; i++) {
        append(end, root->val);
        next(&end);
        next(&root);
    }
}

void remove_adjacent_duplicates(Node *root) {
    while(root && root->next) {
        if(root->val == root->next->val) {
            remove_node(root, 1);
        }
        next(&root);
    }
}

void print(Node *root) {
    if(root == NULL) return;
    do {
        printf("%d\n", root->val);
    } while ((root = root->next));
}

int main() {
    int data[] = {1,2,3,3,4,5,6};
    Node* root = make_node(data[0]);

    for (int i = 1; i < 7; i++) {
        append(root, data[i]);
    }
    printf("Original list:\n");
    print(root);

    printf("Swapping adjacent items:\n");
    swap_adjacent(root);
    print(root);

    printf("Duplicating all items:\n");
    double_list(root);
    print(root);

    printf("Removing adjacent duplicates:\n");
    remove_adjacent_duplicates(root);
    print(root);

    free_to_tail(&root);


    //print(root);

    //print(root);
    /*
    print(root);
    insert_node(root, 5, 8);
    print(root);
    remove_node(root, 5);
    prepend(&root, 20);
    printf("\n\n");
    print(root);
    free_to_tail(&root);
    print(root);
     */
}