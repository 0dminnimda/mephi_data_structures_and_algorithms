#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#define SIZE_VAR size
#include "void_operations.h"

typedef int (*comparator)(const void *, const void *);

static void sort(void *ptr, void *buff, size_t len, size_t size,
                 comparator comp) {
    if (len <= 1) return;

    size_t mid = len / 2;
    sort(ptr, buff, mid, size, comp);
    sort(add(ptr, mid), add(buff, mid), len - mid, size, comp);

    void *lhs = ptr, *lhs_end = add(ptr, mid);
    void *rhs = lhs_end, *rhs_end = add(ptr, len);
    void *last = buff;

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

    size_t sorted = distance(buff, last);
    memcpy(add(ptr, sorted), lhs, distance(lhs, lhs_end) * size);
    memcpy(ptr, buff, sorted * size);
    // what's left in rhs (if any) is already in ptr, no need to copy it around
}

error_t merge_sort(void *ptr, size_t length, size_t size, comparator compare) {
    void *buffer = NEW(buffer, length * size);
    sort(ptr, buffer, length, size, compare);
    free(buffer);
    return 0;
}
