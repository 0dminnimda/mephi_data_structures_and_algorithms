#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>

#include "../lab4b/tree.c"

#define MAX_LINE_LENGTH 1024

unsigned int convert(const char *st) {
  const char *x;
  for (x = st; *x; x++) {
    if (!isdigit(*x)) {
      return 0L; // Return 0 if the string contains non-digit characters
    }
  }
  return (strtoul(st, NULL, 10));
}

char *strip(char *str) {
    while (*str && isspace(*str)) str++;
    char *result = str;
    while (*str && !isspace(*str)) str++;
    *str = '\0';
    return result;
}

void add_to_tree(Tree* tree, unsigned int key, char* filename, int lineno, int num_in_line) {
    char value[MAX_LINE_LENGTH];
    snprintf(value, MAX_LINE_LENGTH, "%s:%d:%d", filename, lineno, num_in_line);
    add_key(tree, key, value);
}

bool load_file(Tree* tree, char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: could not open file '%s'\n", filename);
        return false;
    }
    char line[MAX_LINE_LENGTH];
    int lineno = 1;
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        char* num_str = strtok(line, ",");
        int num_in_line = 1;
        while (num_str) {
            errno = 0;
            unsigned long int num = strtoul(num_str, NULL, 10);
            if (num > UINT_MAX || errno == ERANGE) {
                printf("Invalid number at %s:%d:%d\n", filename, lineno, num_in_line);
            } else {
                add_to_tree(tree, (unsigned int)num, filename, lineno, num_in_line);
                num_str = strtok(NULL, ",");
                num_in_line++;
            }
        }
        lineno++;
    }
    fclose(file);
    return true;
}

int main() {
    Tree* tree = create_tree();
    char filename[MAX_LINE_LENGTH];
    printf("Enter filename: ");
    fgets(filename, MAX_LINE_LENGTH, stdin);

    if (!load_file(tree, strip(filename))) {
        destroy_tree(tree);
        return 0;
    }

    char raw_input[MAX_LINE_LENGTH];
    while (1) {
        printf("Enter number to search for: ");
        fgets(raw_input, MAX_LINE_LENGTH, stdin);
        char *input = strip(raw_input);

        if (strcmp(input, "q") == 0) break;

        errno = 0;
        unsigned long int num = strtoul(input, NULL, 10);
        if (num > UINT_MAX || errno == ERANGE) {
            printf("Invalid number\n");
            continue;
        }

        Node* node = find_key(tree, (unsigned int)num);
        if (node) {
            printf("%s\n", node->value);
        } else {
            printf("Number not found\n");
        }
    }

    destroy_tree(tree);
    return 0;
}
