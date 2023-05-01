#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/string_builder.h"

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

Node* create_node(unsigned int key, const char* value) {
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

Node* add_node(Node* node, unsigned int key, const char* value) {
    if (node == NULL) { return create_node(key, value); }
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
    if (node == NULL || node->key == key) { return node; }
    if (key < node->key) { return find_node(node->left, key); }
    return find_node(node->right, key);
}

Node* find_min_node(Node* node) {
    if (node == NULL) { return NULL; }
    if (node->left == NULL) { return node; }
    return find_min_node(node->left);
}

Node* remove_node(Node* node, unsigned int key) {
    if (node == NULL) { return NULL; }
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

Node* remove_node_loop(Node* root, unsigned int key) {
    Node* parent = NULL;
    Node* current = root;

    // Find the node to be removed and its parent
    while (current != NULL && current->key != key) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (current == NULL) {
        return root;  // Node to be removed not found
    }

    // Case 1: Node with only one child or no child
    if (current->left == NULL || current->right == NULL) {
        Node* child = current->left ? current->left : current->right;

        // If the node to be removed is the root
        if (parent == NULL) {
            root = child;
        } else if (parent->left == current) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    } else {
        // Case 2: Node with two children
        Node* min_right_node = current->right;
        Node* min_right_node_parent = current;

        // Find the inorder successor (minimum node in the right subtree)
        while (min_right_node->left != NULL) {
            min_right_node_parent = min_right_node;
            min_right_node = min_right_node->left;
        }

        // Replace the key and value of the node to be removed
        unsigned int tmp_key = min_right_node->key;
        char* tmp_value = malloc(strlen(min_right_node->value) + 1);
        strcpy(tmp_value, min_right_node->value);

        current->key = tmp_key;
        free(current->value);
        current->value = tmp_value;

        // Remove the inorder successor
        if (min_right_node_parent->left == min_right_node) {
            min_right_node_parent->left = min_right_node->right;
        } else {
            min_right_node_parent->right = min_right_node->right;
        }

        // Destroy the inorder successor
        destroy_node(min_right_node);
        return root;
    }

    // Destroy the removed node
    if (current != NULL) { destroy_node(current); }

    return root;
}

Node* find_max_diff_helper(Node* root, unsigned int key, Node* max_diff_node,
                           int max_diff) {
    if (root == NULL) { return max_diff_node; }

    int diff = abs((int)(root->key - key));
    if (diff > max_diff) {
        max_diff = diff;
        max_diff_node = root;
    }

    Node* left_max_diff_node =
        find_max_diff_helper(root->left, key, max_diff_node, max_diff);
    int left_diff = abs((int)(left_max_diff_node->key - key));
    if (left_diff > max_diff) {
        max_diff = left_diff;
        max_diff_node = left_max_diff_node;
    }

    return find_max_diff_helper(root->right, key, max_diff_node, max_diff);
}

Node* find_max_diff(Node* root, unsigned int key) {
    return find_max_diff_helper(root, key, NULL, -1);
}

void print_node(Node* node) { printf("%u: '%s'\n", node->key, node->value); }

void print_indent(size_t indent) {
    for (size_t i = 0; i < indent; ++i) { printf("    "); }
}

#define TRAVERSAL_ALL ((unsigned int)(-1))

void inorder_traversal(Node* node, unsigned int key, size_t indent) {
    if (node == NULL) { return; }
    inorder_traversal(node->right, key, indent + 1);
    if (key == TRAVERSAL_ALL) {
        print_indent(indent);
        print_node(node);
    } else if (node->key > key) {
        print_node(node);
    }
    inorder_traversal(node->left, key, indent + 1);
}

void free_tree(Node* node) {
    if (node == NULL) { return; }
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

bool add_key(Tree* tree, unsigned int key, char* value) {
    tree->root = add_node(tree->root, key, value);
    tree->size++;
    return true;
}

void remove_key(Tree* tree, unsigned int key) {
    tree->root = remove_node(tree->root, key);
    tree->size--;
}

void inorder(Tree* tree, unsigned int key) { inorder_traversal(tree->root, key, 0); }

Node* find_key(Tree* tree, unsigned int key) { return find_node(tree->root, key); }

Node* max_diff(Tree* tree, unsigned int key) { return find_max_diff(tree->root, key); }

bool import(Tree* tree, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        TREE_ERROR("Error: File not found\n");
        return false;
    }

    // Insert the the table
    unsigned int key;
    char info[100];
    while (fscanf(file, "%u\n%99[^\r\n]", &key, info) == 2) {
        if (!add_key(tree, key, info)) {
            fclose(file);
            return false;
        }
    }
    fclose(file);
    return true;
}

void dump_dot_traversal(Node* parent, Node* node, FILE* file) {
    if (node == NULL) {
        if (!parent) return;
        if (!parent->left && !parent->right) return;  // one child have to exist

        fprintf(file, "n%u_null [shape=doublecircle, fillcolor=red];\n", parent->key);
        fprintf(file, "n%u -> n%u_null;\n", parent->key, parent->key);
    } else {
        fprintf(file, "n%u [label=\"%u\\n%s\"];\n", node->key, node->key, node->value);
        if (parent) fprintf(file, "n%u -> n%u;\n", parent->key, node->key);

        dump_dot_traversal(node, node->left, file);
        dump_dot_traversal(node, node->right, file);
    }
}

bool dump_dot(Tree* tree, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        TREE_ERROR("Error: File not found\n");
        return false;
    }

    fprintf(file, "digraph G {\n");
    fprintf(file, "node [label=\"\", shape=ellipse, style=filled];\n\n");
    dump_dot_traversal(NULL, tree->root, file);
    fprintf(file, "}\n");

    fclose(file);
    return true;
}

bool to_image(Tree* tree, const char* filename) {
    StringBuilder* sb = sb_create(0);

    sb_append(sb, filename);
    sb_append(sb, ".dot");
    if (!dump_dot(tree, sb_to_string(sb))) {
        sb_free(sb);
        return false;
    }
    sb_clear(sb);

    sb_append(sb, "dot -Tpng -Gdpi=300 ");
    sb_append(sb, filename);
    sb_append(sb, ".dot -o ");
    sb_append(sb, filename);
    printf("Running '%s'\n", sb_to_string(sb));
    int result = system(sb_to_string(sb));
    sb_free(sb);

    if (result != 0) TREE_ERROR("ERROR: Could not create an image\n");
    return result == 0;
}
