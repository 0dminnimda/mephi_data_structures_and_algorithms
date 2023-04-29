#include <stdio.h>

#include "common/input.h"
#include "tree.c"

#define IS_COMMAND(input, name, short_name) \
    (strcmp(input, name) == 0 || strcmp(input, short_name) == 0)

#define CLEAR() \
    while ((getchar()) != '\n') {}

#define SCAN(n, ...)                      \
    if (scanf(__VA_ARGS__) != n) {        \
        CLEAR();                          \
        printf("Error: invalid input\n"); \
        continue;                         \
    } else {                              \
        CLEAR();                          \
    }

int main() {
    Tree* tree = create_tree();
    char* input = NULL;

    bool run = true;
    while (run) {
        printf("\033[32m");  // set text color to green
        printf(
            "Enter command "
            "(add/delete/find/max_diff/traverse/output/dump_dot/image/import_file/"
            "quit):\n");
        printf("\033[0m");  // reset text color to default
        printf("> ");

        free(input);
        input = read_line();

        if (input == NULL) {
            run = false;
        } else if (IS_COMMAND(input, "add", "a")) {
            printf("Enter key: ");
            unsigned int key;
            SCAN(1, "%u", &key);

            printf("Enter info: ");
            char* info = read_line();

            add_key(tree, key, info);

            free(info);
        } else if (IS_COMMAND(input, "delete", "d")) {
            printf("Enter key: ");
            unsigned int key;
            SCAN(1, "%u", &key);

            remove_key(tree, key);
        } else if (IS_COMMAND(input, "find", "f")) {
            printf("Enter key: ");
            unsigned int key;
            SCAN(1, "%u", &key);

            Node* node = find_key(tree, key);
            if (node) {
                print_node(node);
            } else {
                printf("Key not found.\n");
            }
        } else if (IS_COMMAND(input, "max_diff", "m")) {
            printf("Enter key: ");
            unsigned int key;
            SCAN(1, "%u", &key);

            Node* node = max_diff(tree, key);
            if (node) {
                print_node(node);
            } else {
                printf("No node found.\n");
            }
        } else if (IS_COMMAND(input, "traverse", "t")) {
            printf("Enter key: ");
            unsigned int key;
            SCAN(1, "%u", &key);

            inorder(tree, key);
        } else if (IS_COMMAND(input, "output", "o")) {
            inorder(tree, TRAVERSAL_ALL);
        } else if (IS_COMMAND(input, "dump_dot", "dd")) {
            printf("Enter filename: ");
            char* filename = read_line();

            dump_dot(tree, filename);

            free(filename);
        } else if (IS_COMMAND(input, "image", "i")) {
            printf("Enter filename: ");
            char* filename = read_line();

            to_image(tree, filename);

            free(filename);
        } else if (IS_COMMAND(input, "import_file", "if")) {
            printf("Enter filename: ");
            char* filename = read_line();

            import(tree, filename);

            free(filename);
        } else if (IS_COMMAND(input, "quit", "q")) {
            run = false;
        } else {
            printf("\033[0;31m");  // set color to red
            printf("Yo mama is so stpid!!! >:(\n");
            printf("Ter aino command '%s'\n", input);
            printf("\033[0m");  // reset color to default
        }
    }

    free(input);
    destroy_tree(tree);
    return 0;
}
