#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int alloc_flag = 0;

int allocation_succeeded(void) { return alloc_flag; }

void *allocate(size_t size) {
    alloc_flag = 0;
    void *var = calloc(size, 1);
    if (size && var == NULL) {
        alloc_flag = -1;
    }
    return var;
}

#define NEW(size)                            \
    allocate(size);                          \
    if (allocation_succeeded()) {            \
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

void free_matrix(Matrix mat) {
    for (int i = 0; i < mat.count; i++)
        if (mat.lines[i].items) free(mat.lines[i].items);
    free(mat.lines);
}

int input_matrix(Matrix *mat) {
    int len;
    while (1) {
        printf("Input number of lines: ");
        if (get_int(&len)) return -1;
        if (len >= 0) break;
        printf("Incorrect input, try again!\n");
    }
    mat->count = len;

    mat->lines = NEW(len * sizeof(Line));
    for (int i = 0; i < len; i++) {
        int len;
        while (1) {
            printf("Input line length: ");
            if (get_int(&len)) {
                free_matrix(*mat);
                return -1;
            }
            if (len >= 0) break;
            printf("Incorrect input, try again!\n");
        }
        mat->lines[i].count = len;
        double *items = NEW(sizeof(double) * len);
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

double line_min(Line line) {
    if (!line.count) return 0;
    double result = line.items[0];
    for (size_t i = 1; i < line.count; i++)
        if (line.items[i] < result) result = line.items[i];
    return result;
}

typedef struct {
    double value;
    size_t index;
} Pair;

int pair_cmp(const void *a, const void *b) {
    double lhs = ((Pair *)a)->value;
    double rhs = ((Pair *)b)->value;
    return (lhs > rhs) - (lhs < rhs);
}

int sort_by_min(Matrix mat) {
    Pair *pairs = malloc(sizeof(Pair) * mat.count);
    for (size_t i = 0; i < mat.count; i++)
        pairs[i] = (Pair){.value = line_min(mat.lines[i]), .index = i};
    qsort(pairs, mat.count, sizeof(Pair), pair_cmp);

    Line *old_lines = NEW(sizeof(Line) * mat.count);
    memcpy(old_lines, mat.lines, sizeof(Line) * mat.count);

    for (size_t i = 0; i < mat.count; i++) {
        mat.lines[i] = old_lines[pairs[i].index];
    }
    free(old_lines);
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
    if (sort_by_min(mat)) return -1;
    output("Result matrix", mat);
    free_matrix(mat);
    return 0;
}
