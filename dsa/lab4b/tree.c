struct Node *create_node(unsigned int key, const char *value);
struct Node *balance_for_add(struct Node *node, unsigned int key);
struct Node *balance_for_remove(struct Node *node);

#define BALANCE_FOR_REMOVE(node, key) balance_for_remove(node)
#define BALANCE_FOR_ADD(node, key, value) balance_for_add(node, key)

#define EXTRA_BINARY_TREE_FIELDS int height;
#define create_node old_create_node
#include "../lab4/tree.c"
#undef create_node

#undef max
int max(int a, int b) { return (a > b) ? a : b; }

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

Node *balance_for_add(Node *node, unsigned int key) {
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

Node *balance_for_remove(Node *node) {
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
