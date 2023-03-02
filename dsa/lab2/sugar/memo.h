#include <stdio.h>

#include "error.h"
#include "platform.h"

#define MEMORY_ERROR_TYPE MAKE_ERROR_TYPE("MemoryError")
#define MEMORY_ERROR(message) MAKE_ERROR(MEMORY_ERROR_TYPE, message)

#if IS_PLATFORM(WINDOWS) || IS_PLATFORM(APPLE) || IS_PLATFORM(LINUX)
size_t memory_size(const void *m);
#else
#error Unsupported platform
#endif

void *sugar_malloc(size_t size);

#define NEW_OR_FAIL(var, size)  \
    (var = sugar_malloc(size)); \
    if ((size) && var == NULL)

#define NEW(var, size) \
    NEW_OR_FAIL(var, size) return MEMORY_ERROR("Bad allocation");

void *nulled_realloc(void *ptr, size_t old_size, size_t new_size);

#define RENEW_OR_FAIL(var, size)                         \
    (var = nulled_realloc(var, memory_size(var), size)); \
    if ((size) && var == NULL)

#define RENEW(var, size) \
    RENEW_OR_FAIL(var, size) return MEMORY_ERROR("Bad reallocation");

void *sugar_delete(void *ptr);

// DELETE returns a pointer back
#define DELETE(x) sugar_delete(x)

// DELETE_INPLACE returns a pointer back and is a valid lvalue
#define DELETE_INPLACE(x) *(DELETE(x), &x)
