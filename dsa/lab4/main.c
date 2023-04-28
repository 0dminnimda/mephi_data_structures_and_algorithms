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

void add_node_loop(Tree* tree, unsigned int key, const char* value) {
    Node* new_node = create_node(key, value);
    if (tree->root == NULL) {
        tree->root = new_node;
        return;
    }

    Node* current = tree->root;
    Node* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            // Key already exists, update the value and destroy the new node
            free(current->value);
            current->value = malloc(strlen(value) + 1);
            strcpy(current->value, value);
            destroy_node(new_node);
            return;
        }
    }

    if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
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

Node* remove_node_less(Node* root, unsigned int key) {
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
        return root; // Node to be removed not found
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
        Node* min_right_node = find_min_node(current->right);
        unsigned int tmp_key = min_right_node->key;
        char* tmp_value = malloc(strlen(min_right_node->value) + 1);
        strcpy(tmp_value, min_right_node->value);

        // Remove the inorder successor
        root = remove_node_less(root, min_right_node->key);

        // Replace the key and value of the node to be removed
        current->key = tmp_key;
        free(current->value);
        current->value = tmp_value;
    }

    // Destroy the removed node
    if (current != NULL) {
        destroy_node(current);
    }

    return root;
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
        return root; // Node to be removed not found
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
    if (current != NULL) {
        destroy_node(current);
    }

    return root;
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
