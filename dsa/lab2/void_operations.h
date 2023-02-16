#ifndef VOID_OPERATIONS_H_
#define VOID_OPERATIONS_H_

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

static inline void swap(void *ptr1, void *ptr2, size_t size) {
    char *lhs = (char *)ptr1, *rhs = (char *)ptr2;
    while (size--) {
        char tmp = *lhs;
        *lhs++ = *rhs;
        *rhs++ = tmp;
    }
}

static inline void copy(void *destination, const void *source, size_t size) {
    char *lhs = (char *)destination, *rhs = (char *)source;
    while (size--) *lhs++ = *rhs++;
}

#ifdef SIZE_VAR

#define add(ptr, offset) add(ptr, offset, SIZE_VAR)
#define inc(ptr) inc(ptr, SIZE_VAR)
#define dec(ptr) dec(ptr, SIZE_VAR)
#define distance(ptr1, ptr2) distance(ptr1, ptr2, SIZE_VAR)
#define swap(ptr1, ptr2) swap(ptr1, ptr2, SIZE_VAR)
#define copy(ptr1, ptr2) copy(ptr1, ptr2, SIZE_VAR)

#endif  // SIZE_VAR

#endif  // VOID_OPERATIONS_H_
