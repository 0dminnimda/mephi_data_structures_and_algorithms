#include <stdio.h>
#include "table.c"
#include "common/input.h"

const static char *names[] = {
    "input",
    "delete",
    "deleteall",
    "find",
    "findspecific",
    "output",
    "file",
    "quit",
};

const static char *short_names[] = {
    "i",
    "d",
    "da",
    "f",
    "fs",
    "o",
    "f",
    "q",
};

#define IS_COMMAND(input, index) (strcmp(input, names[index]) == 0 || strcmp(input, short_names[index]) == 0)

int main() {
    Table *table = create_table(10);

    bool run = true;
    while (run) {
        printf("\033[32m"); // set text color to green
        printf("Enter command (input/delete/deleteall/find/findspecific/output/file/quit):\n");
        printf("\033[0m"); // reset text color to default
        printf("> ");
        char *input = read_line();

        if (input == NULL) {
            run = false;
        } else if (IS_COMMAND(input, 0)) {
            KeyType key;
            Item info;
            printf("Enter key and info:\n");
            scanf("%u %u", &key, &info);
            while ((getchar()) != '\n');
            insert(table, key, info);
        } else if (IS_COMMAND(input, 1)) {
            KeyType key;
            RelType release;
            printf("Enter key and release:\n");
            scanf("%u %u", &key, &release);
            while ((getchar()) != '\n');
            delete_one_version(table, key, release);
        } else if (IS_COMMAND(input, 2)) {
            KeyType key;
            printf("Enter key:\n");
            scanf("%u", &key);
            while ((getchar()) != '\n');
            delete_all_versions(table, key);
        } else if (IS_COMMAND(input, 3)) {
            KeyType key;
            printf("Enter key:\n");
            scanf("%u", &key);
            while ((getchar()) != '\n');
            Node *node = search_all(table, key);
            if (node == NULL) {
                printf("No keys found\n");
            }
            while (node != NULL) {
                printf("Key: %u, Release: %u, Info: %u\n", key, node->release, *(node->info));
                node = search_all(NULL, key);
            }
        } else if (IS_COMMAND(input, 4)) {
            KeyType key;
            RelType release;
            printf("Enter key and release:\n");
            scanf("%u %u", &key, &release);
            while ((getchar()) != '\n');
            Node *node = search_spesific(table, key, release);
            if (node != NULL) {
                printf("Key: %u, Release: %u, Info: %u\n", key, node->release, *(node->info));
            }
        } else if (IS_COMMAND(input, 5)) {
            output(table);
        } else if (IS_COMMAND(input, 6)) {
            char filename[100];
            printf("Enter filename:\n");
            scanf("%s", filename);
            while ((getchar()) != '\n');
            import(table, filename);
        } else if (IS_COMMAND(input, 7)) {
            run = false;
        } else {
            printf("\033[0;31m"); // set color to red
            printf("Yo mama is so stpid!!! >:(\n");
            printf("Ter aino command '%s'\n", input);
            printf("\033[0m"); // reset color to default
        }

        free(input);
    }
    free_table(table);
    return 0;
}
