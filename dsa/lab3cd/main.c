#include <stdio.h>
#include "table.c"
#include "common/input.h"

int main() {
    Table *table = create_table(10);

    bool run = true;
    while (run) {
        printf("Enter command (inp/del/delall/find/out/file/quit):\n");
        printf("> ");
        char *input = read_line();
        if (strcmp(input, "inp") == 0) {
            KeyType key;
            Item info;
            printf("Enter key and info:\n");
            scanf("%u %u", &key, &info);
            insert(table, key, info);
        } else if (strcmp(input, "del") == 0) {
            KeyType key;
            RelType release;
            printf("Enter key and release:\n");
            scanf("%u %u", &key, &release);
            delete_one_version(table, key, release);
        } else if (strcmp(input, "delall") == 0) {
            KeyType key;
            printf("Enter key:\n");
            scanf("%u", &key);
            delete_all_versions(table, key);
        } else if (strcmp(input, "find") == 0) {
            KeyType key;
            printf("Enter key:\n");
            scanf("%u", &key);
            Node *node = search_all(table, key);
            while (node != NULL) {
                printf("Key: %u, Release: %u, Info: %u\n", key, node->release, *(node->info));
                node = search_all(NULL, key);
            }

            // KeyType key;
            // printf("Enter key:\n");
            // scanf("%u", &key);
            // KeySpace *ks = search(table, key);
            // if (ks != NULL && ks->node != NULL) {
            //     Node *node = ks->node;
            //     printf("Key: %u, Release: %u, Info: %u\n", key, node->release, *(node->info));
            // } else {
            //     printf("Key not found\n");
            // }
        } else if (strcmp(input, "out") == 0) {
            output(table);
        } else if (strcmp(input, "file") == 0) {
            char filename[100];
            printf("Enter filename:\n");
            scanf("%s", filename);
            import(table, filename);
        } else if (strcmp(input, "quit") == 0) {
            run = false;
        } else {
            printf("\033[0;31m"); // set color to red
            printf("Yo mama is so stpid!!! >:(\n");
            printf("\033[0m"); // reset color to default
        }

        free(input);
    }
    free_table(table);
    return 0;
}
