struct Node *create_node(unsigned int key, const char *value);
struct Node *balance_for_add(struct Node *node, unsigned int key);
struct Node *balance_for_remove(struct Node *node);

#define BALANCE_FOR_REMOVE(node, key) balance_for_remove(node)
#define BALANCE_FOR_ADD(node, key, value) balance_for_add(node, key)

#define EXTRA_BINARY_TREE_FIELDS int height;
#define INIT_EXTRA_BINARY_TREE_FIELDS(node) node->height = 1;

#define inorder_traversal old_inorder_traversal
#define inorder old_inorder
#include "../lab4/tree.c"
#undef inorder_traversal
#undef inorder

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

void inorder_traversal(Node *node, unsigned int key, size_t indent) {
    if (node == NULL) { return; }
    inorder_traversal(node->right, key, indent + 1);
    if (key == TRAVERSAL_ALL) {
        print_indent(indent);
        print_node(node);
    } else if (node->key <= key) {
        print_node(node);
    }
    inorder_traversal(node->left, key, indent + 1);
}

void inorder(Tree *tree, unsigned int key) { inorder_traversal(tree->root, key, 0); }
