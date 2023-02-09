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
        if (result == 1)
            return 1;
        if (result < 0)
            return 0;
        printf("%s\n", "Invalid input, try again!");
        scanf("%*c");
    }
}

int get_double(double *a) {
    while (1) {
        int result = scanf("%lf", a);
        if (result == 1)
            return 1;
        if (result < 0)
            return 0;
        printf("%s\n", "Invalid input, try again!");
        scanf("%*c");
    }
}

void free_matrix(Matrix mat) {
    for (int i = 0; i < mat.count; ++i)
        free(mat.lines[i].items);
    free(mat.lines);
}

int input_matrix(Matrix *rm) {
    const char *pr = "";
    int m;
    do {
        printf("%s\n", pr);
        printf("Enter number of count: --> ");
        pr = "You are wrong; repeat, please!";
        if (get_int(&m) == 0)
            return 0;
    } while (m < 1);
    rm->count = m;
    rm->lines = (Line *)calloc(m, sizeof(Line));
    for (int i = 0; i < rm->count; ++i) {
        pr = "";
        do {
            printf("%s\n", pr);
            printf("Enter number of items in line %d: --> ", i + 1);
            pr = "You are wrong; repeat, please!";
            if (get_int(&m) == 0) {
                rm->count = i;
                free_matrix(*rm);
                return 0;
            }
        } while (m < 1);
        rm->lines[i].count = m;
        double *p = (double *)malloc(sizeof(double) * m);
        rm->lines[i].items = p;
        printf("Enter items for Matrix️ line #%d:\n", i + 1);
        for (int j = 0; j < m; ++j, ++p)
            if (get_double(p) == 0) {
                rm->count = i + 1;
                free_matrix(*rm);
                return 0;
            }
    }
    return 1;
}

void output(const char *msg, Matrix mat) {
    printf("%s:\n", msg);
    for (int i = 0; i < mat.count; ++i) {
        double *p = mat.lines[i].items;
        for (int j = 0; j < mat.lines[i].count; ++j, ++p)
            printf("%10lf ", *p);
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
    for (size_t i = 0; i < mat.count; i++) {
        pairs[i] = (Pair){line_min(mat.lines[i]), i};
    }
    qsort(pairs, mat.count, sizeof(Pair), pair_cmp);

    // Line *old_lines = malloc(sizeof(Line) * mat.count);
    Line *old_lines = NEW(old_lines, sizeof(Line) * mat.count);
    memcpy(old_lines, mat.lines, sizeof(Line) * mat.count);

    for (size_t i = 0; i < mat.count; i++) {
        printf("%d\n", pairs[i].index);
        mat.lines[i] = old_lines[pairs[i].index];
    }
    free(old_lines);
    return 0;
}

int main() {
    Matrix matr = {0, NULL};
    double res;
    if (input_matrix(&matr) == 0) {
        printf("%s\n", "End of file occured");
        return 1;
    }
    output("Source matrix", matr);
    if (sort_by_min(matr)) return -1;
    output("Result matrix", matr);
    free_matrix(matr);
    return 0;
}
