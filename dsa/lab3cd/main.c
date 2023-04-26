#include <stdio.h>
#include "table.c"
#include "common/input.h"

#define IS_COMMAND(input, name, short_name) (strcmp(input, name) == 0 || strcmp(input, short_name) == 0)

#define CLEAR() while ((getchar()) != '\n');

#define SCAN(n, ...) if (scanf(__VA_ARGS__) != n) { CLEAR(); printf("Error: invalid input\n"); continue; } else { CLEAR(); }

int main() {
    Table *table = create_table(10);
    char *input = NULL;

    bool run = true;
    while (run) {
        printf("\033[32m"); // set text color to green
        printf("Enter command (input/delete/deleteall/find/findall/findallcopy/output/file/quit):\n");
        printf("\033[0m"); // reset text color to default
        printf("> ");

        free(input);
        input = read_line();

        if (input == NULL) {
            run = false;
        } else if (IS_COMMAND(input, "input", "i")) {
            KeyType key;
            Item info;
            printf("Enter key and info:\n");
            SCAN(2, "%u %u", &key, &info);
            insert(table, key, info);
        } else if (IS_COMMAND(input, "delete", "d")) {
            KeyType key;
            RelType release;
            printf("Enter key and release:\n");
            SCAN(2, "%u %u", &key, &release);
            delete_one_version(table, key, release);
        } else if (IS_COMMAND(input, "deleteall", "da")) {
            KeyType key;
            printf("Enter key:\n");
            SCAN(1, "%u", &key);
            delete_all_versions(table, key);
        } else if (IS_COMMAND(input, "find", "f")) {
            KeyType key;
            RelType release;
            printf("Enter key and release:\n");
            SCAN(2, "%u %u", &key, &release);
            Node *node = search_spesific(table, key, release);
            if (node != NULL) {
                printf("Key: %u, Release: %u, Info: %u\n", key, node->release, *(node->info));
            }
        } else if (IS_COMMAND(input, "findall", "fa")) {
            KeyType key;
            printf("Enter key:\n");
            SCAN(1, "%u", &key);
            Node *node = search_all(table, key);
            if (node == NULL) {
                printf("No keys found\n");
            }
            while (node != NULL) {
                printf("Key: %u, Release: %u, Info: %u\n", key, node->release, *(node->info));
                node = search_all(NULL, key);
            }
        } else if (IS_COMMAND(input, "findallcopy", "fac")) {
            KeyType key;
            printf("Enter key:\n");
            SCAN(1, "%u", &key);
            Table *table_copy = search_all_copy(table, key);
            output(table_copy);
            free_table(table_copy);
        } else if (IS_COMMAND(input, "output", "o")) {
            output(table);
        } else if (IS_COMMAND(input, "file", "f")) {
            char filename[100];
            printf("Enter filename:\n");
            SCAN(1, "%s", filename);
            import(table, filename);
        } else if (IS_COMMAND(input, "quit", "q")) {
            run = false;
        } else {
            printf("\033[0;31m"); // set color to red
            printf("Yo mama is so stpid!!! >:(\n");
            printf("Ter aino command '%s'\n", input);
            printf("\033[0m"); // reset color to default
        }
    }

    free(input);
    free_table(table);
    return 0;
}
