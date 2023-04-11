#include <stdio.h>
#include "hash_table.h"

int main() {
    Table *table = create_table(10);
    printf("Enter command (i/d/s/o/f/a/q):\n");

    bool run = true;
    while (run) {
        printf("> ");
        char *input = read_line();
        if (strcmp(input, "i") == 0) {
            KeyType key;
            RelType release;
            Item info;
            printf("Enter key, release, and info:\n");
            scanf("%u %u %u", &key, &release, &info);
            insert(table, key, release, info);
        } else if (strcmp(input, "d") == 0) {
            KeyType key;
            RelType release;
            printf("Enter key and release:\n");
            scanf("%u %u", &key, &release);
            delete(table, key, release);
        } else if (strcmp(input, "da") == 0) {
            KeyType key;
            printf("Enter key:\n");
            scanf("%u", &key);
            Node *node = search(table, key);
            if (node != NULL) {
                printf("Key: %u, Release: %u, Info: %u\n", node->key, node->release, *(node->info));
            } else {
                printf("Key not found\n");
            }
        } else if (strcmp(input, "o") == 0) {
            output(table);
        } else if (strcmp(input, "f") == 0) {
            char filename[100];
            printf("Enter filename:\n");
            scanf("%s", filename);
            import(table, filename);
        } else if (strcmp(input, "sa") == 0) {
            KeyType key;
            printf("Enter key:\n");
            scanf("%u", &key);
            Node *node = search_all(table, key);
            while (node != NULL) {
                printf("Key: %u, Release: %u, Info: %u\n", node->key, node->release, *(node->info));
                node = node->next;
            }
        }

        free(input);
    }
    free_table(table);
    return 0;
}
