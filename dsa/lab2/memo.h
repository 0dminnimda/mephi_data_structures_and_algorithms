#include "error.h"

#define NEW_WITH_ERROR(var, size, on_error)  \
    NULL;                                    \
    var = calloc(size, sizeof(char));                   \
    if (size && var == NULL) { on_error; }


#define NEW(var, size) NEW_WITH_ERROR(var, size, { \
    fprintf(stderr, "Bad allocation\n"); return (error_t)(-1); \
})

void *nulled_realloc(void *ptr, size_t old_size, size_t new_size);

#define RENEW_WITH_ERROR(var, size, on_error)  \
    NULL;                                      \
    var = nulled_realloc(size);                \
    if (size && var == NULL) { on_error; }


#define RENEW(var, size) RENEW_WITH_ERROR(var, size, { \
    fprintf(stderr, "Bad reallocation\n"); return (error_t)(-1); \
})

