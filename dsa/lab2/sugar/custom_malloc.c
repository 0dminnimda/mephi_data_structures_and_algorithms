#define CUSTOM_MALLOC_NO_DEFINES
#include "custom_malloc.h"

#include <stdlib.h>
#include <string.h>

#if !(defined(__STDC__) && __STDC_VERSION__ >= 201112L)
typedef double max_align_t;
#endif

/*
Use of max_align_t allows to retain alignment,
with the cost of additional memory usage
Thus returned pointers will be aligned
*/

#define BEGINNING(ptr) (&(((max_align_t *)ptr)[-1]))
#define SIZE(ptr) (((size_t *)BEGINNING(ptr))[0])

void *custom_malloc(size_t size) {
    if (size == 0) return NULL;
    max_align_t *ret = malloc(sizeof(max_align_t) + size);
    if (ret == NULL) return NULL;
    *(size_t *)ret = size;
    return &ret[1];
}

void *custom_calloc(size_t num, size_t size) {
    size = num * size;  // FIXME: no overflow checks
    if (size == 0) return NULL;
    max_align_t *ret = calloc(sizeof(max_align_t) + size, 1);
    if (ret == NULL) return NULL;
    *(size_t *)ret = size;
    return &ret[1];
}

void *custom_realloc(void *ptr, size_t size) {
    if (size == 0) {
        custom_free(ptr);
        return NULL;
    }
    void *beginning = ptr == NULL ? NULL : BEGINNING(ptr);
    max_align_t *ret = realloc(beginning, sizeof(max_align_t) + size);
    if (ret == NULL) return NULL;
    *(size_t *)ret = size;
    return &ret[1];
}

size_t custom_malloc_size(const void *ptr) {
    if (ptr == NULL) return 0;
    return SIZE(ptr);
}

void custom_free(void *ptr) {
    if (ptr == NULL) return;
    free(BEGINNING(ptr));
}
