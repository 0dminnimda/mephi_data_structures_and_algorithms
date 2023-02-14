#include <stdio.h>

#include "error.h"

#define POSSIBLY_UNUSED(var)                                   \
    _Pragma("clang diagnostic push")                           \
        _Pragma("clang diagnostic ignored \"-Wunused-value\"") \
            var _Pragma("clang diagnostic pop")

#define NEW_WITH_ERROR(var, size, on_error) \
    NULL;                                   \
    var = calloc(size, sizeof(char));       \
    if (size && var == NULL) {              \
        on_error;                           \
    }

#define NEW(var, size)                       \
    NEW_WITH_ERROR(var, size, {              \
        fprintf(stderr, "Bad allocation\n"); \
        return (error_t)(-1);                \
    })

void *nulled_realloc(void *ptr, size_t old_size, size_t new_size);

#define RENEW_WITH_ERROR(var, prev_size, size, on_error) \
    POSSIBLY_UNUSED(var);                                \
    var = nulled_realloc(var, size, prev_size);          \
    if (size && var == NULL) {                           \
        on_error;                                        \
    }

#define RENEW(var, prev_size, size)            \
    RENEW_WITH_ERROR(var, prev_size, size, {   \
        fprintf(stderr, "Bad reallocation\n"); \
        return (error_t)(-1);                  \
    })
