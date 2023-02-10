#include <stdbool.h>
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

int get_int(int *a) {
    while (1) {
        int result = scanf("%d", a);
        if (result == 1) return 0;
        if (result != 0) return -1;
        printf("%s\n", "Invalid input, try again!");
        scanf("%*c");
    }
}

int get_double(double *a) {
    while (1) {
        int result = scanf("%lf", a);
        if (result == 1) return 0;
        if (result != 0) return -1;
        printf("%s\n", "Invalid input, try again!");
        scanf("%*c");
    }
}

int retrying_int_input(const char *msg, int *a, bool positive) {
    while (1) {
        printf("%s", msg);
        if (get_int(a)) return -1;
        if (positive && *a >= 0) return 0;
        printf("Incorrect input, try again!\n");
    }
}

void free_matrix(Matrix mat) {
    for (int i = 0; i < mat.count; i++)
        if (mat.lines[i].items) free(mat.lines[i].items);
    free(mat.lines);
}

int input_matrix(Matrix *mat) {
    if (retrying_int_input("Input number of lines: ", &(mat->count), true))
        return -1;

    mat->lines = NEW(mat->lines, mat->count * sizeof(Line));
    for (int i = 0; i < mat->count; i++) {

        int len;
        if (retrying_int_input("Input line length: ", &len, true)) {
            free_matrix(*mat);
            return -1;
        };
        mat->lines[i].count = len;

        double *items = NEW(items, len * sizeof(double));
        mat->lines[i].items = items;

        printf("Input items: ");
        for (int j = 0; j < len; j++)
            if (get_double(items++)) {
                free_matrix(*mat);
                return -1;
            }
    }

    return 0;
}

void output(const char *msg, Matrix mat) {
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

int sort_max_line(Matrix mat) {
    int max_index = 0;
    double max_value = 0;
    for (size_t i = 0; i < mat.count; i++) {
        double sum = line_sum(mat.lines[i]);
        if (max_value < sum) {
            max_value = sum;
            max_index = i;
        }
    }

    Line max_line = mat.lines[max_index];
    qsort(max_line.items, max_line.count, sizeof(double), double_cmp);

    return 0;
}

int main() {
    Matrix mat = {0, NULL};
    double res;
    if (input_matrix(&mat)) {
        printf("%s\n", "End of file occurred");
        return -1;
    }
    output("Source matrix", mat);
    if (sort_max_line(mat)) return -1;
    output("Result matrix", mat);
    free_matrix(mat);
    return 0;
}
