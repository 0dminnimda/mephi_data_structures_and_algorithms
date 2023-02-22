#include <stdio.h>

#include "error.h"
#include "platform.h"

#define MEMORY_ERROR_TYPE "MemoryError"
#define MEMORY_ERROR(message) MAKE_ERROR(MEMORY_ERROR_TYPE, message)

#define NEW_WITH_ERROR(var, size, on_error) \
    (var = calloc(size, sizeof(char)));     \
    if ((size) && var == NULL) {            \
        on_error;                           \
    }

#define NEW(var, size) \
    NEW_WITH_ERROR(var, size, return MEMORY_ERROR("Bad allocation");)

#if PLATFORM == Windows || PLATFORM == Apple || PLATFORM == Unix
size_t memory_size(const void *m);
#else
#error Unsupported platform
#endif

void *nulled_realloc(void *ptr, size_t old_size, size_t new_size);

#define RENEW_WITH_ERROR(var, old_size, size, on_error) \
    (var = nulled_realloc(var, old_size, size));        \
    if ((size) && var == NULL) {                        \
        on_error;                                       \
    }

#define RENEW(var, old_size, size)        \
    RENEW_WITH_ERROR(var, old_size, size, \
                     return MEMORY_ERROR("Bad reallocation");)
