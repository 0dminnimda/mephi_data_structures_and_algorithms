#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#define SIZE_VAR size
#include "void_operations.h"

typedef int (*comparator)(const void *, const void *);

static void sort(void *ptr, void *buff, size_t lo, size_t hi, size_t size,
                 comparator comp) {
    if (lo >= hi - 1) return;

    size_t mid = (lo + hi) / 2;
    sort(ptr, buff, lo, mid, size, comp);
    sort(ptr, buff, mid, hi, size, comp);

    void *lhs = add(ptr, lo), *lhs_end = add(ptr, mid);
    void *rhs = add(ptr, mid), *rhs_end = add(ptr, hi);
    void *last = add(buff, lo);

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

    size_t sorted = distance(add(buff, lo), last);
    memcpy(add(ptr, lo + sorted), lhs, distance(lhs, lhs_end) * size);
    memcpy(add(ptr, lo), add(buff, lo), sorted * size);
    // what's left in rhs (if any) is already in ptr, no need to copy it around
}

error_t merge_sort(void *ptr, size_t length, size_t size, comparator comp) {
    void *buffer = NEW(buffer, length * size);
    sort(ptr, buffer, 0, length, size, comp);
    free(buffer);
    return 0;
}
