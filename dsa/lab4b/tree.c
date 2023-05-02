#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/string_builder.h"

#define EXTRA_BINARY_TREE_FIELDS int height;
#define create_node old_create_node
#define add_node old_add_node
#define remove_node old_remove_node
#include "../lab4/tree.c"
#undef create_node
#undef add_node
#undef remove_node

#ifndef NO_TABLE_PRINT_ERRORS
    #define TREE_ERROR(msg) fprintf(stderr, msg)
#else
    #define TREE_ERROR(msg)
#endif

int node_height(Node *node) {
    if (node == NULL) return 0;
    return node->height;
}

int balance_factor(Node *node) {
    if (node == NULL) return 0;
    return node_height(node->left) - node_height(node->right);
}

void update_height(Node *node) {
    node->height = 1 + max(node_height(node->left), node_height(node->right));
}

Node *rotate_right(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    update_height(y);
    update_height(x);

    return x;
}

Node *rotate_left(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    update_height(x);
    update_height(y);

    return y;
}

Node *create_node(unsigned int key, const char *value) {
    Node *node = old_create_node(key, value);
    node->height = 1;
    return node;
}

Node *add_node(Node *node, unsigned int key, const char *value) {
    node = old_add_node(node, key, value);

    update_height(node);

    int balance = balance_factor(node);

    if (balance > 1 && key < node->left->key) {
        // Left-Left case
        return rotate_right(node);
    }
    if (balance < -1 && key > node->right->key) {
        // Right-Right case
        return rotate_left(node);
    }
    if (balance > 1 && key > node->left->key) {
        // Left-Right case
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && key < node->right->key) {
        // Right-Left case
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

Node *remove_node(Node *node, unsigned int key) {
    node = old_remove_node(node, key);

    update_height(node);

    int balance = balance_factor(node);

    if (balance > 1 && balance_factor(node->left) >= 0) {
        // Left-Left case
        return rotate_right(node);
    }
    if (balance < -1 && balance_factor(node->right) <= 0) {
        // Right-Right case
        return rotate_left(node);
    }
    if (balance > 1 && balance_factor(node->left) < 0) {
        // Left-Right case
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && balance_factor(node->right) > 0) {
        // Right-Left case
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}
