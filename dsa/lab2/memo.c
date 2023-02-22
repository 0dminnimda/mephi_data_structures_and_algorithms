#include "memo.h"

#include <stdlib.h>
#include <string.h>

#if PLATFORM == Windows
#include <malloc.h>
size_t memory_size(const void *m) { return _msize((void *)m); }
#elif PLATFORM == Apple
#include <malloc/malloc.h>
size_t memory_size(const void *m) { return malloc_size(m); }
#elif PLATFORM == Unix
#include <malloc.h>
size_t memory_size(const void *m) { return malloc_usable_size((void *)m); }
#endif

void *nulled_realloc(void *ptr, size_t old_size, size_t new_size) {
    ptr = realloc(ptr, new_size);
    if (ptr != NULL && new_size > old_size)
        memset((char *)ptr + old_size, 0, new_size - old_size);
    return ptr;
}
