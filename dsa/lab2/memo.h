#include <stdio.h>

#include "error.h"
#include "platform.h"

#define MEMORY_ERROR_TYPE MAKE_ERROR_TYPE("MemoryError")
#define MEMORY_ERROR(message) MAKE_ERROR(MEMORY_ERROR_TYPE, message)

#define NEW_ON_ERROR(var, size)         \
    (var = calloc(size, sizeof(char))); \
    if ((size) && var == NULL)

#define NEW(var, size) \
    NEW_ON_ERROR(var, size) return MEMORY_ERROR("Bad allocation");

#if IS_PLATFORM(WINDOWS) || IS_PLATFORM(APPLE) || IS_PLATFORM(UNIX)
size_t memory_size(const void *m);
#else
#error Unsupported platform
#endif

void *nulled_realloc(void *ptr, size_t old_size, size_t new_size);

#define RENEW_ON_ERROR(var, size)                        \
    (var = nulled_realloc(var, memory_size(var), size)); \
    if ((size) && var == NULL)

#define RENEW(var, size) \
    RENEW_ON_ERROR(var, size) return MEMORY_ERROR("Bad reallocation");
