#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEW(var, size)                       \
    NULL;                                    \
    var = calloc(size, 1);                   \
    if (size && var == NULL) {               \
        fprintf(stderr, "Bad allocation\n"); \
        return -1;                           \
    }

typedef struct {
    int count;
    double *items;
} Line;

typedef struct {
    int count;
    Line *lines;
} Matrix;

void clear_buffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_int(const char *msg, int *a) {
    while (1) {
        printf("%s", msg);
        int result = scanf("%d", a);
        if (result == 1) return 0;
        if (result != 0) return -1;
        printf("Invalid input, try again!\n");
        clear_buffer();
    }
}

int get_non_negative_int(const char *msg, int *a) {
    while (1) {
        if (get_int(msg, a)) return -1;
        if (*a >= 0) return 0;
        printf("Number should be non negative, try again!\n");
    }
}

int get_double(const char *msg, double *a) {
    while (1) {
        printf("%s", msg);
        int result = scanf("%lf", a);
        if (result == 1) return 0;
        if (result != 0) return -1;
        printf("Invalid input, try again!\n");
        clear_buffer();
    }
}

void free_matrix(Matrix mat) {
    Line *lines = mat.lines;
    for (int i = 0; i < mat.count; i++, lines++)
        if (lines->items) free(lines->items);
    free(mat.lines);
}

int input_matrix(Matrix *mat) {
    if (get_non_negative_int("Input number of lines: ", &(mat->count)))
        return -1;

    mat->lines = NEW(mat->lines, mat->count * sizeof(Line));
    Line *lines = mat->lines;
    for (int i = 0; i < mat->count; i++, lines++) {
        int len;
        if (get_non_negative_int("Input line length: ", &len)) {
            free_matrix(*mat);
            return -1;
        }
        lines->count = len;

        double *items = NEW(items, len * sizeof(double));
        lines->items = items;

        printf("Input items: ");
        for (int j = 0; j < len; j++)
            if (get_double("", items++)) {
                free_matrix(*mat);
                return -1;
            }
    }

    return 0;
}

void print_matrix(const char *msg, Matrix mat) {
    printf("%s:\n", msg);
    for (int i = 0; i < mat.count; i++) {
        for (int j = 0; j < mat.lines[i].count; j++)
            printf("%10lf ", mat.lines[i].items[j]);
        printf("\n");
    }
}

double line_sum(Line line) {
    double result = 0;
    for (size_t i = 0; i < line.count; i++) result += line.items[i];
    return result;
}

int double_cmp(const void *a, const void *b) {
    double x = *(double *)a;
    double y = *(double *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

size_t max_sum_line_index(Matrix mat) {
    size_t index = 0;
    double value = 0;
    for (size_t i = 0; i < mat.count; i++) {
        double sum = line_sum(mat.lines[i]);
        if (value < sum) {
            value = sum;
            index = i;
        }
    }
    return index;
}

double line_max(Line *line) {
    if (line->count <= 0) return 0;
    double result = line->items[0];
    for (size_t i = 1; i < line->count; i++) {
        if (result < line->items[i]) {
            result = line->items[i];
        }
    }
    return result;
}

size_t max_item_line_index(Matrix mat) {
    if (mat.count <= 0) return 0;
    size_t index = 0;
    double value = line_max(mat.lines + 0);
    for (size_t i = 1; i < mat.count; i++) {
        double max_item = line_max(mat.lines + i);
        if (value < max_item) {
            value = max_item;
            index = i;
        }
    }
    return index;
}

int sort_max_line(Matrix mat) {
    Line max_line = mat.lines[max_item_line_index(mat)];
    qsort(max_line.items, max_line.count, sizeof(double), double_cmp);

    return 0;
}

int main() {
    Matrix mat = {0, NULL};
    if (input_matrix(&mat)) {
        printf("End of file occurred\n");
        return -1;
    }
    print_matrix("Source matrix", mat);
    if (sort_max_line(mat)) return -1;
    print_matrix("Result matrix", mat);
    free_matrix(mat);
    return 0;
}
