#include "error.h"

#define PARSE_ERROR_TYPE MAKE_ERROR_TYPE("ParseError")
#define PARSE_ERROR(message) MAKE_ERROR(PARSE_ERROR_TYPE, message)

typedef unsigned long ulong;

Error parse_long(char **str, long *value);
Error parse_ulong(char **str, ulong *value);
