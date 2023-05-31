#include <ctype.h>
#include <stdio.h>
#include <sys/stat.h>

#include "common/input.h"
#include "table.c"

bool file_exists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

char *strip(char *str) {
    while (*str && isspace(*str)) str++;
    char *result = str;
    while (*str && !isspace(*str)) str++;
    *str = '\0';
    return result;
}

#define IS_COMMAND(input, name, short_name) \
    (strcmp(input, name) == 0 || strcmp(input, short_name) == 0)

#define CLEAR()                 \
    while ((getchar()) != '\n') \
        ;

#define SCAN(n, ...)                      \
    if (scanf(__VA_ARGS__) != n) {        \
        CLEAR();                          \
        printf("Error: invalid input\n"); \
        continue;                         \
    } else {                              \
        CLEAR();                          \
    }

int main() {
    char *input = NULL;
    bool run = true;

#if USE_FILE
    Table *table = NULL;

    printf("Enter file name: ");
    char *raw_file_name = read_line();
    char *file_name = strip(raw_file_name);
    if (!file_exists(file_name)) {
        Table *tmp = create_table(10);
        if (!dump_table(tmp, file_name)) { run = false; }
        free_table(tmp);
    }
#else
    Table *table = create_table(10);
#endif

    while (run) {
        printf("\033[32m");  // set text color to green
        printf(
            "Enter command "
            "(input/delete/deleteall/find/findall/findallcopy/output/file/quit):\n");
        printf("\033[0m");  // reset text color to default
        printf("> ");

        free(input);
        input = read_line();

#if USE_FILE
        free_table(table);
        if ((table = load_table(file_name)) == NULL) {
            printf("Failed to load from the file\n");
            break;
        }
#endif

        if (input == NULL) {
            break;
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
                printf("Key: %u, Release: %u, Info: %u\n", key, node->release,
                       *(node->info));
            }
        } else if (IS_COMMAND(input, "findall", "fa")) {
            KeyType key;
            printf("Enter key:\n");
            SCAN(1, "%u", &key);
            Node *node = search_all(table, key);
            if (node == NULL) { printf("No keys found\n"); }
            while (node != NULL) {
                printf("Key: %u, Release: %u, Info: %u\n", key, node->release,
                       *(node->info));
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
            break;
        } else {
            printf("\033[0;31m");  // set color to red
            printf("Yo mama is so stpid!!! >:(\n");
            printf("Ter aino command '%s'\n", input);
            printf("\033[0m");  // reset color to default
        }

#if USE_FILE
        if (!dump_table(table, file_name)) { printf("Failed to save into a file\n"); }
#endif
    }

#if USE_FILE
    free(raw_file_name);
#endif
    free(input);
    free_table(table);
    return 0;
}
