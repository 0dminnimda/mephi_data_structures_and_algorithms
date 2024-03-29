#include "memo.h"

#include <stdlib.h>
#include <string.h>

#if !defined(MEMO_USE_CUSTOM_MALLOC) && \
    !(IS_PLATFORM(WINDOWS) || IS_PLATFORM(APPLE) || IS_PLATFORM(LINUX))
#define MEMO_USE_CUSTOM_MALLOC
#endif

#ifdef MEMO_USE_CUSTOM_MALLOC
#include "custom_malloc.h"
#define unsafe_memory_size(m) malloc_size(m)
#elif IS_PLATFORM(WINDOWS)
#include <malloc.h>
#define unsafe_memory_size(m) _msize((void *)m)
#elif IS_PLATFORM(APPLE)
#include <malloc/malloc.h>
#define unsafe_memory_size(m) malloc_size(m)
#elif IS_PLATFORM(LINUX)
#include <malloc.h>
#define unsafe_memory_size(m) malloc_usable_size((void *)m)
#else
#error Unreachable
#endif

size_t memory_size(const void *m) {
    if (m == NULL) return 0;
    return unsafe_memory_size(m);
}

void *sugar_new(size_t size) { return calloc(size, sizeof(char)); }

static inline void *nulled_realloc(void *ptr, size_t old_size,
                                   size_t new_size) {
    ptr = realloc(ptr, new_size);
    if (ptr != NULL && new_size > old_size)
        memset((char *)ptr + old_size, 0, new_size - old_size);
    return ptr;
}

void *sugar_renew(void *ptr, size_t size) {
    return nulled_realloc(ptr, memory_size(ptr), size);
}

void *sugar_delete(void *ptr) {
    free(ptr);
    return ptr;
}
