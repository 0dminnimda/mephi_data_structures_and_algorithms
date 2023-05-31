#define CUSTOM_MALLOC_NO_DEFINES
#include "custom_malloc.h"

#include <stdlib.h>
#include <string.h>

// if the alignment is not critical for your system + you want the smallest size
// than #define ALIGNED_SIZE sizeof(size_t) in your user code

#if defined(__STDC__) && __STDC_VERSION__ >= 201112L
#define ALIGNED_SIZE _Alignof(max_align_t)
#elif !defined(ALIGNED_SIZE)
#define ALIGNED_SIZE sizeof(long double)
#endif

/*
Use of ALIGNED_SIZE allows to retain alignment,
with the cost of additional memory usage
Thus returned pointers will be aligned
*/

// TODO: create gist + https://stackoverflow.com/a/36734227/12141949

#define BEGINNING(ptr) ((char *)ptr - ALIGNED_SIZE)
#define SIZE(ptr) (((size_t *)BEGINNING(ptr))[0])

void *custom_malloc(size_t size) {
    if (size == 0) return NULL;
    void *ret = malloc(ALIGNED_SIZE + size);
    if (ret == NULL) return NULL;
    *(size_t *)ret = size;
    return (char *)ret + ALIGNED_SIZE;
}

void *custom_calloc(size_t num, size_t size) {
    size = num * size;  // FIXME: no overflow checks
    if (size == 0) return NULL;
    void *ret = calloc(ALIGNED_SIZE + size, 1);
    if (ret == NULL) return NULL;
    *(size_t *)ret = size;
    return (char *)ret + ALIGNED_SIZE;
}

void *custom_realloc(void *ptr, size_t size) {
    if (size == 0) {
        custom_free(ptr);
        return NULL;
    }
    void *beginning = ptr == NULL ? NULL : BEGINNING(ptr);
    void *ret = realloc(beginning, ALIGNED_SIZE + size);
    if (ret == NULL) return NULL;
    *(size_t *)ret = size;
    return (char *)ret + ALIGNED_SIZE;
}

size_t custom_malloc_size(const void *ptr) {
    if (ptr == NULL) return 0;
    return SIZE(ptr);
}

void custom_free(void *ptr) {
    if (ptr == NULL) return;
    free(BEGINNING(ptr));
}
