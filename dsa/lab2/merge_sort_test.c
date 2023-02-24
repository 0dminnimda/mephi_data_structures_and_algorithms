#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int error_t;

#define ON_ERROR(expr, on_error)  \
    do {                          \
        error_t error = expr;     \
        if (error != 0) on_error; \
    } while (0);

#define WITH_ERROR(expr) ON_ERROR(expr, return error)


#include <stdio.h>


#define POSSIBLY_UNUSED(var)                                   \
    _Pragma("clang diagnostic push")                           \
        _Pragma("clang diagnostic ignored \"-Wunused-value\"") \
            var _Pragma("clang diagnostic pop")

#define NEW_WITH_ERROR(var, size, on_error) \
    NULL;                                   \
    var = calloc(size, sizeof(char));       \
    if (size && var == NULL) {              \
        on_error;                           \
    }

#define NEW(var, size)                       \
    NEW_WITH_ERROR(var, size, {              \
        fprintf(stderr, "Bad allocation\n"); \
        return (error_t)(-1);                \
    })


#include <stdlib.h>

static inline void *add(const void *ptr, long long offset, size_t size) {
    return (char *)ptr + offset * (long long)size;
}

static inline void *inc(void **ptr, size_t size) {
    return *ptr = add(*ptr, 1, size);
}

static inline void *dec(void **ptr, size_t size) {
    return *ptr = add(*ptr, -1, size);
}

static inline size_t distance(const void *ptr1, const void *ptr2, size_t size) {
    return abs(((char *)ptr1 - (char *)ptr2) / (long long)size);
}


// static error_t swap_s(void *a, void *b, size_t size) {
//     char *temp = NEW(temp, size);
//     if (temp != NULL) {
//         memmove(temp, a, size);
//         memmove(a, b, size);
//         memmove(b, temp, size);
//     }
//     free(temp);
// }

static inline void swap(void *a, void *b, size_t size) {
    char *lhs = (char *)a, *rhs = (char *)b;
    while (size--) {
        char tmp = *lhs;
        *lhs++ = *rhs;
        *rhs++ = tmp;
    }
}


#define add(ptr, offset) add(ptr, offset, size)
#define inc(ptr) inc(ptr, size)
#define dec(ptr) dec(ptr, size)
#define distance(ptr1, ptr2) distance(ptr1, ptr2, size)
#define swap(ptr1, ptr2) swap(ptr1, ptr2, size)

typedef int (*comparator)(const void *, const void *);

#define memcpyp(a, b, s) \
    printf("memcpy(%zu, %zu, %zu)\n", (size_t)a / size, (size_t)b / size, s); \
    memcpy(a, b, s);

static void sort(void *ptr, void *buff, size_t lo, size_t hi, size_t size,
                 comparator comp) {
    if (lo >= hi - 1) return;

    size_t mid = (lo + hi) / 2;
    sort(ptr, buff, lo, mid, size, comp);
    sort(ptr, buff, mid, hi, size, comp);

    printf("\n%zu %zu | %zu %zu\n", lo, hi, (size_t)ptr / size, (size_t)buff / size);
    printf("> ");
    for (size_t i = lo; i < hi; i++) printf("%d ", ((int *)ptr)[i]);
    printf("| ");
    for (size_t i = lo; i < hi; i++) printf("%d ", ((int *)buff)[i]);
    printf("\n");

    void *lhs = add(ptr, lo), *lhs_end = add(ptr, mid);
    void *rhs = add(ptr, mid), *rhs_end = add(ptr, hi);
    void *last = add(buff, lo);
    printf("%zu\n", (size_t)last);

    while (lhs < lhs_end && rhs < rhs_end) {
        if (comp(lhs, rhs) <= 0) {
            memcpy(last, lhs, size);
            // swap(last, lhs);
            inc(&lhs);
        } else {
            memcpy(last, rhs, size);
            // swap(last, rhs);
            inc(&rhs);
        }
        inc(&last);
        printf("%zu\n", (size_t)last);
    }

    printf("> ");
    for (size_t i = lo; i < hi; i++) printf("%d ", ((int *)ptr)[i]);
    printf("| ");
    for (size_t i = lo; i < hi; i++) printf("%d ", ((int *)buff)[i]);
    printf("\n");

    size_t sorted = distance(add(buff, lo), last);
    printf("%zu %zu %zu\n", (size_t)buff, (size_t)last, sorted);
    memcpyp(add(ptr, lo + sorted), lhs, distance(lhs, lhs_end) * size);

    printf("> ");
    for (size_t i = lo; i < hi; i++) printf("%d ", ((int *)ptr)[i]);
    printf("| ");
    for (size_t i = lo; i < hi; i++) printf("%d ", ((int *)buff)[i]);
    printf("\n");

    memcpyp(add(ptr, lo), add(buff, lo), sorted * size);
    // what's left in rhs (if any) is already in ptr, no need to copy it around

    // last = add(ptr, distance(last, buff));
    // while (lhs < add(ptr, mid)) {
    //     swap(last, lhs);
    //     inc(&lhs);
    //     inc(&last);
    // }

    printf("> ");
    for (size_t i = lo; i < hi; i++) printf("%d ", ((int *)ptr)[i]);
    printf("| ");
    for (size_t i = lo; i < hi; i++) printf("%d ", ((int *)buff)[i]);
    printf("\n");
    printf("\n");
}

error_t merge_sort(void *ptr, size_t length, size_t size, comparator comp) {
    void *buffer = NEW(buffer, length * size);
    sort(ptr, buffer, 0, length, size, comp);
    free(buffer);
    return 0;
}


int int_cmp(const void *a, const void *b) {
    int l = *(int *)a;
    int r = *(int *)b;
    return (l > r) - (l < r);
}

int main() {
    int a[] = {60, 33, 95, 70, 76, 56, 29, 98, 57, 38, 22, 65, 18, 45, 90, 56, 79, 59, 87, 52, 8, 26, 33, 4, 54, 38, 21, 79, 38, 59, 78, 14, 50, 1, 41, 92, 14, 4, 82, 10};
    for (size_t i = 0; i < 40; i++) printf("%d ", a[i]);
    printf("\n");
    merge_sort((void *)a, 40, sizeof(int), int_cmp);
    printf("\n");
    for (size_t i = 0; i < 40; i++) printf("%d ", a[i]);
    printf("\n");
}

