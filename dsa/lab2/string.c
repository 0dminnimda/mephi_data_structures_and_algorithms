#include "string.h"

#include <errno.h>
#include <string.h>

#define STR(x) #x

#define GENERATE_PARSE_FUNCTION(type, base_parse_func) \
    Error parse_##type(char **str, type *value) {      \
        char *input = *str;                            \
        type result = base_parse_func(input, str, 10); \
        if (*str == input || errno != 0)               \
            return VALUE_ERROR("Invalid " STR(type));  \
        *value = result;                               \
        return OK;                                     \
    }

GENERATE_PARSE_FUNCTION(long, strtol)
GENERATE_PARSE_FUNCTION(ulong, strtoul)
