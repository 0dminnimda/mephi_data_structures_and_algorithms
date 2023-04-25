#include <stdio.h>
#include "table.c"
#include "common/input.h"

const static char *names[] = {
    "input",
    "delete",
    "deleteall",
    "find",
    "output",
    "file",
    "quit",
};

const static char *short_names[] = {
    "i",
    "d",
    "da",
    "f",
    "o",
    "f",
    "q",
};

#define IS_COMMAND(input, index) (strcmp(input, names[index]) == 0 || strcmp(input, short_names[index]) == 0)

int main() {
    Table *table = create_table(10);

    bool run = true;
    while (run) {
        // printf("Enter command (i[input]/d[delete]/da[deleteall]/f[find]/o[output]/f[file]/q[quit]):\n");
        printf("Enter command (input/delete/deleteall/find/output/file/quit):\n");
        printf("> ");
        char *input = read_line();
        if (IS_COMMAND(input, 0)) {
            KeyType key;
            Item info;
            printf("Enter key and info:\n");
            scanf("%u %u\n", &key, &info);
            insert(table, key, info);
        } else if (IS_COMMAND(input, 1)) {
            KeyType key;
            RelType release;
            printf("Enter key and release:\n");
            scanf("%u %u", &key, &release);
            delete_one_version(table, key, release);
        } else if (IS_COMMAND(input, 2)) {
            KeyType key;
            printf("Enter key:\n");
            scanf("%u", &key);
            delete_all_versions(table, key);
        } else if (IS_COMMAND(input, 3)) {
            KeyType key;
            printf("Enter key:\n");
            scanf("%u", &key);
            Node *node = search_all(table, key);
            while (node != NULL) {
                printf("Key: %u, Release: %u, Info: %u\n", key, node->release, *(node->info));
                node = search_all(NULL, key);
            }
        } else if (IS_COMMAND(input, 4)) {
            output(table);
        } else if (IS_COMMAND(input, 5)) {
            char filename[100];
            printf("Enter filename:\n");
            scanf("%s", filename);
            import(table, filename);
        } else if (IS_COMMAND(input, 6)) {
            run = false;
        } else {
            printf("\033[0;31m"); // set color to red
            printf("Yo mama is so stpid!!! >:(\n");
            printf("'%s' %lu %d\n", input, strlen(input), input==NULL);
            printf("\033[0m"); // reset color to default
        }

        free(input);
    }
    free_table(table);
    return 0;
}
