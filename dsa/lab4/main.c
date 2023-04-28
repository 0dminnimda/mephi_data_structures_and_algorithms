#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NO_TABLE_PRINT_ERRORS
    #define TREE_ERROR(msg) fprintf(stderr, msg)
#else
    #define TREE_ERROR(msg)
#endif

typedef struct Node {
    unsigned int key;
    char* value;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct Tree {
    Node* root;
    int size;
} Tree;

Node* create_node(unsigned int key, char* value) {
    Node* node = malloc(sizeof(Node));
    node->key = key;
    node->value = malloc(strlen(value) + 1);
    strcpy(node->value, value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void destroy_node(Node* node) {
    free(node->value);
    free(node);
}

Node* add_node(Node* node, unsigned int key, char* value) {
    if (node == NULL) {
        return create_node(key, value);
    }
    if (key < node->key) {
        node->left = add_node(node->left, key, value);
    } else if (key > node->key) {
        node->right = add_node(node->right, key, value);
    } else {
        TREE_ERROR("Error: duplicate key\n");
    }
    return node;
}

Node* find_node(Node* node, unsigned int key) {
    if (node == NULL || node->key == key) {
        return node;
    }
    if (key < node->key) {
        return find_node(node->left, key);
    }
    return find_node(node->right, key);
}

Node* find_min_node(Node* node) {
    if (node == NULL) {
        return NULL;
    }
    if (node->left == NULL) {
        return node;
    }
    return find_min_node(node->left);
}

Node* remove_node(Node* node, unsigned int key) {
    if (node == NULL) {
        return NULL;
    }
    if (key < node->key) {
        node->left = remove_node(node->left, key);
    } else if (key > node->key) {
        node->right = remove_node(node->right, key);
    } else {
        if (node->left == NULL) {
            Node* right_child = node->right;
            destroy_node(node);
            return right_child;
        } else if (node->right == NULL) {
            Node* left_child = node->left;
            destroy_node(node);
            return left_child;
        } else {
            Node* min_right_node = find_min_node(node->right);
            unsigned int tmp_key = min_right_node->key;
            char* tmp_value = malloc(strlen(min_right_node->value) + 1);
            strcpy(tmp_value, min_right_node->value);
            node->right = remove_node(node->right, min_right_node->key);
            node->key = tmp_key;
            free(node->value);
            node->value = tmp_value;
        }
    }
    return node;
}

void print_node(Node* node) {
    printf("%u: %s\n", node->key, node->value);
}

void inorder_traversal(Node* node, unsigned int key, size_t indent) {
    if (node == NULL) {
        return;
    }
    inorder_traversal(node->right, key, indent + 1);
    if (key == 0 || node->key > key) {
        for (size_t i = 0; i < indent; ++i) {
            printf("    ");
        }
        print_node(node);
    }
    inorder_traversal(node->left, key, indent + 1);
}

void free_tree(Node* node) {
    if (node == NULL) {
        return;
    }
    free_tree(node->left);
    free_tree(node->right);
    destroy_node(node);
}

Tree* create_tree() {
    Tree* tree = malloc(sizeof(Tree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void destroy_tree(Tree* tree) {
    free_tree(tree->root);
    free(tree);
}

void add(Tree* tree, unsigned int key, char* value) {
    tree->root = add_node(tree->root, key, value);
    tree->size++;
}

void remove_key(Tree* tree, unsigned int key) {
    tree->root = remove_node(tree->root, key);
    tree->size--;
}

void inorder(Tree* tree, unsigned int key) {
    inorder_traversal(tree->root, key, 0);
}

Node* find(Tree* tree, unsigned int key) {
    return find_node(tree->root, key);
}

Node* find_max_diff(Tree* tree, unsigned int key) {
    Node* node = tree->root;
    Node* result = NULL;
    while (node != NULL) {
        if (result == NULL || abs(node->key - key) > abs(result->key - key)) {
            result = node;
        }
        if (node->key > key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return result;
}

int main() {
    Tree* tree = create_tree();

    add(tree, 10, "ten");
    add(tree, 5, "five");
    add(tree, 15, "fifteen");
    add(tree, 3, "three");
    add(tree, 7, "seven");
    add(tree, 12, "twelve");
    add(tree, 20, "twenty");

    printf("Inorder traversal:\n");
    inorder(tree, 0);

    printf("\nFind key 7:\n");
    Node* found_node = find(tree, 7);
    if (found_node) {
        print_node(found_node);
    } else {
        printf("Key not found.\n");
    }

    printf("\nRemove key 5:\n");
    remove_key(tree, 5);
    inorder(tree, 0);

    printf("\nFind max diff from key 8:\n");
    Node* max_diff_node = find_max_diff(tree, 8);
    if (max_diff_node) {
        print_node(max_diff_node);
    } else {
        printf("No node found.\n");
    }

    destroy_tree(tree);

    return 0;
}
