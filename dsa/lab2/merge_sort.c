#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#define SIZE_VAR size
#include "void_operations.h"

typedef int (*comparator)(const void *, const void *);

static void sort(void *ptr, void *buffer, size_t length, size_t size,
                 comparator comp) {
    if (length <= 1) return;

    size_t middle = length / 2;
    sort(ptr, buffer, middle, size, comp);
    sort(add(ptr, middle), add(buffer, middle), length - middle, size, comp);

    void *lhs = ptr, *lhs_end = add(ptr, middle);
    void *rhs = lhs_end, *rhs_end = add(ptr, length);
    void *last = buffer;

    while (lhs < lhs_end && rhs < rhs_end) {
        if (comp(lhs, rhs) <= 0) {
            copy(last, lhs);
            inc(&lhs);
        } else {
            copy(last, rhs);
            inc(&rhs);
        }
        inc(&last);
    }

    size_t sorted = distance(buffer, last);
    memcpy(add(ptr, sorted), lhs, distance(lhs, lhs_end) * size);
    memcpy(ptr, buffer, sorted * size);
    // what's left in rhs (if any) is already in ptr, no need to copy it around
}

error_t merge_sort(void *ptr, size_t length, size_t size, comparator compare) {
    void *buffer = NEW(buffer, length * size);
    sort(ptr, buffer, length, size, compare);
    free(buffer);
    return 0;
}
