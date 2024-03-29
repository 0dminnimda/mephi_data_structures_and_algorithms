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

#ifndef EXTRA_BINARY_TREE_FIELDS
    #define EXTRA_BINARY_TREE_FIELDS
#endif

#ifndef INIT_EXTRA_BINARY_TREE_FIELDS
    #define INIT_EXTRA_BINARY_TREE_FIELDS(node)
#endif

typedef struct Node {
    unsigned int key;
    char *value;
    struct Node *left;
    struct Node *right;
    EXTRA_BINARY_TREE_FIELDS
} Node;

typedef struct Tree {
    Node *root;
    int size;
} Tree;

Node *create_node(unsigned int key, const char *value) {
    Node *node = calloc(1, sizeof(Node));
    node->key = key;
    node->value = calloc(strlen(value) + 1, sizeof(char));
    strcpy(node->value, value);
    node->left = NULL;
    node->right = NULL;
    INIT_EXTRA_BINARY_TREE_FIELDS(node);
    return node;
}

void destroy_node(Node *node) {
    free(node->value);
    free(node);
}

#ifndef BALANCE_FOR_ADD
    #define BALANCE_FOR_ADD(node, key, value) node
#endif

Node *add_node(Node *node, unsigned int key, const char *value) {
    if (node == NULL) { return create_node(key, value); }
    if (key < node->key) {
        node->left = add_node(node->left, key, value);
    } else if (key > node->key) {
        node->right = add_node(node->right, key, value);
    } else {
        TREE_ERROR("Error: duplicate key\n");
        return node;
    }
    return BALANCE_FOR_ADD(node, key, value);
}

Node *find_node(Node *node, unsigned int key) {
    if (node == NULL || node->key == key) { return node; }
    if (key < node->key) { return find_node(node->left, key); }
    return find_node(node->right, key);
}

Node *find_min_node(Node *node) {
    if (node == NULL) { return NULL; }
    if (node->left == NULL) { return node; }
    return find_min_node(node->left);
}

Node *find_max_node(Node *node) {
    if (node == NULL) { return NULL; }
    if (node->right == NULL) { return node; }
    return find_max_node(node->right);
}

#ifndef BALANCE_FOR_REMOVE
    #define BALANCE_FOR_REMOVE(node, key) node
#endif

Node *remove_node(Node *node, unsigned int key) {
    if (node == NULL) { return NULL; }
    if (key < node->key) {
        node->left = remove_node(node->left, key);
    } else if (key > node->key) {
        node->right = remove_node(node->right, key);
    } else {
        if (node->left == NULL) {
            Node *right_child = node->right;
            destroy_node(node);
            return right_child;
        } else if (node->right == NULL) {
            Node *left_child = node->left;
            destroy_node(node);
            return left_child;
        } else {
            Node *min_right_node = find_min_node(node->right);

            // copy the min node into the current node
            node->key = min_right_node->key;
            // swap the string so the removal of min will destroy the current string
            // and to eliminate the need to allocate memory for min node string
            char *tmp = min_right_node->value;
            min_right_node->value = node->value;
            node->value = tmp;

            node->right = remove_node(node->right, min_right_node->key);
        }
    }
    return BALANCE_FOR_REMOVE(node, key);
}

Node *find_max_diff(Node *node, unsigned int key) {
    Node *min_node = find_min_node(node);
    Node *max_node = find_max_node(node);

    if (key <= min_node->key) return max_node;
    if (key >= max_node->key) return min_node;

    unsigned int diff_min = key - min_node->key;
    unsigned int diff_max = max_node->key - key;
    if (diff_min < diff_max) return max_node;
    return min_node;
}

void print_node(Node *node) { printf("%u: '%s'\n", node->key, node->value); }

void print_indent(size_t indent) {
    for (size_t i = 0; i < indent; ++i) { printf("    "); }
}

#define TRAVERSAL_ALL ((unsigned int)(-1))

void inorder_traversal(Node *node, unsigned int key, size_t indent) {
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

unsigned int visit_node(Node *node) {
    if (node == NULL) { return 0; }
    unsigned int result = node->key;
    result += visit_node(node->right);
    result += visit_node(node->left);
    return result;
}

void free_tree(Node *node) {
    if (node == NULL) { return; }
    free_tree(node->left);
    free_tree(node->right);
    destroy_node(node);
}

Tree *create_tree() {
    Tree *tree = malloc(sizeof(Tree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void destroy_tree(Tree *tree) {
    free_tree(tree->root);
    free(tree);
}

bool add_key(Tree *tree, unsigned int key, char *value) {
    tree->root = add_node(tree->root, key, value);
    tree->size++;
    return true;
}

void remove_key(Tree *tree, unsigned int key) {
    tree->root = remove_node(tree->root, key);
    tree->size--;
}

void inorder(Tree *tree, unsigned int key) { inorder_traversal(tree->root, key, 0); }

void visit_tree(Tree *tree) { visit_node(tree->root); }

Node *find_key(Tree *tree, unsigned int key) { return find_node(tree->root, key); }

Node *max_diff(Tree *tree, unsigned int key) { return find_max_diff(tree->root, key); }

bool import(Tree *tree, const char *filename) {
    FILE *file = fopen(filename, "r");
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

void dump_dot_traversal(Node *parent, Node *node, FILE *file) {
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

bool dump_dot(Tree *tree, const char *filename) {
    FILE *file = fopen(filename, "w");
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

bool to_image(Tree *tree, const char *filename) {
    StringBuilder *sb = sb_create(0);

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
