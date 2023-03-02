#include <stdio.h>

#include "error.h"
#include "platform.h"

#define MEMORY_ERROR_TYPE MAKE_ERROR_TYPE("MemoryError")
#define MEMORY_ERROR(message) MAKE_ERROR(MEMORY_ERROR_TYPE, message)

size_t memory_size(const void *m);
void *sugar_new(size_t size);
void *sugar_renew(void *ptr, size_t size);
void *sugar_delete(void *ptr);

#define NEW_OR_FAIL(var, size)  \
    (var = sugar_new(size)); \
    if ((size) && var == NULL)

#define NEW(var, size) \
    NEW_OR_FAIL(var, size) return MEMORY_ERROR("Bad allocation");

#define RENEW_OR_FAIL(var, size)      \
    (var = sugar_renew(var, size)); \
    if ((size) && var == NULL)

#define RENEW(var, size) \
    RENEW_OR_FAIL(var, size) return MEMORY_ERROR("Bad reallocation");

// DELETE returns a pointer back
#define DELETE(x) sugar_delete(x)

// DELETE_INPLACE returns a pointer back and is a valid lvalue
#define DELETE_INPLACE(x) *(DELETE(x), &x)
