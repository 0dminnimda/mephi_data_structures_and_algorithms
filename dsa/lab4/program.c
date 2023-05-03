#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <error.h>

#include "tree.c"

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

void add_to_tree(Tree* tree, char* filename, int line_num, int num_in_line) {
    char value[MAX_LINE_LENGTH];
    snprintf(value, MAX_LINE_LENGTH, "%s:%d:%d", filename, line_num, num_in_line);
    add_key(tree, num_in_line, value);
}

void load_file(Tree* tree, char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: could not open file %s\n", filename);
        return;
    }
    char line[MAX_LINE_LENGTH];
    int line_num = 1;
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        char* num_str = strtok(line, ",");
        int num_in_line = 1;
        while (num_str) {
            errno = 0;
            unsigned long int num = strtoul(num_str, NULL, 10);
            if (num > UINT_MAX || errno == ERANGE) {
                // error
            } else {
                add_to_tree(tree, filename, line_num, num_in_line);
                num_str = strtok(NULL, ",");
                num_in_line++;
            }
        }
        line_num++;
    }
    fclose(file);
}

int main() {
    Tree* tree = create_tree();
    char filename[MAX_LINE_LENGTH];
    printf("Enter filename: ");
    fgets(filename, MAX_LINE_LENGTH, stdin);

    load_file(tree, strip(filename));

    char raw_input[MAX_LINE_LENGTH];
    while (1) {
        printf("Enter number to search for: ");
        fgets(raw_input, MAX_LINE_LENGTH, stdin);
        char *input = strip(raw_input);
        if (strlen(input) == 1) {  // only newline character entered
            break;
        }
        Node* node = find_key(tree, atoi(input));
        if (node) {
            printf("%s\n", node->value);
        } else {
            printf("Number not found\n");
        }
    }

    destroy_tree(tree);
    return 0;
}
