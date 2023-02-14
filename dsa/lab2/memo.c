#include <stdlib.h>
#include <string.h>


void *nulled_realloc(void *ptr, size_t old_size, size_t new_size) {
    ptr = realloc(ptr, new_size);
    if (ptr != NULL && new_size > old_size)
        memset((char *)ptr + old_size, 0, new_size - old_size);
    return ptr;
}
