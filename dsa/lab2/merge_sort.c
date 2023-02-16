#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#define SIZE_VAR size
#include "void_operations.h"

typedef int (*comparator)(const void *, const void *);

static void sort(void *data, void *result, size_t length, size_t size,
                 comparator comp) {
    if (length <= 1) return;

    size_t middle = length / 2;
    sort(result, data, middle, size, comp);
    sort(add(result, middle), add(data, middle), length - middle, size, comp);
    // ⬆ sorts data section given that result contains the copy of the data

    void *lhs = data, *lhs_end = add(data, middle);
    void *rhs = lhs_end, *rhs_end = add(data, length);

    while (lhs < lhs_end && rhs < rhs_end) {
        if (comp(lhs, rhs) <= 0) {
            copy(result, lhs);
            inc(&lhs);
        } else {
            copy(result, rhs);
            inc(&rhs);
        }
        inc(&result);
    }

    // don't perform a syscall when possible
    if (lhs != lhs_end)
        memcpy(result, lhs, distance(lhs, lhs_end) * size);
    else if (rhs != rhs_end)
        memcpy(result, rhs, distance(rhs, rhs_end) * size);
    // sorted elemets are already in result, no need to copy into data
}

error_t merge_sort(void *ptr, size_t length, size_t size, comparator compare) {
    void *copy = NEW(copy, length * size);
    memcpy(copy, ptr, length * size);

    sort(copy, ptr, length, size, compare);

    free(copy);
    return 0;
}
