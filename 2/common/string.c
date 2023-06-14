#include "string.h"

#include <errno.h>
#include <limits.h>
#include <string.h>

#ifndef SIZE_T_MAX
#define SIZE_T_MAX ((size_t)(-1))
#endif

#define STR(x) #x

#define GENERATE_PARSE_FUNCTION(type, base_parse_func) \
    Error parse_##type(char **str, type *value) {      \
        char *input = *str;                            \
        type result = base_parse_func(input, str, 10); \
        if (*str == input || errno != 0)               \
            return PARSE_ERROR("Invalid " STR(type));  \
        *value = result;                               \
        return OK;                                     \
    }

GENERATE_PARSE_FUNCTION(long, strtol)
GENERATE_PARSE_FUNCTION(llong, strtoll)
GENERATE_PARSE_FUNCTION(ulong, strtoul)
GENERATE_PARSE_FUNCTION(ullong, strtoull)

Error parse_size_t(char **str, size_t *value) {
    Error error;
    ullong result;
    TRY(parse_ullong(str, &result))
    CATCH(PARSE_ERROR_TYPE) return PARSE_ERROR("Invalid " STR(size_t));
    CATCH_N_THROW

#if SIZE_MAX < ULLONG_MAX
    if (result >= SIZE_T_MAX) return PARSE_ERROR("Invalid " STR(size_t));
#endif

    *value = (size_t)result;
    return OK;
}
