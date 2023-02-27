#include "sugar/sugar.h"

#define PARSE_ERROR_TYPE MAKE_ERROR_TYPE("ParseError")
#define PARSE_ERROR(message) MAKE_ERROR(PARSE_ERROR_TYPE, message)

#define WHITESPACE " \t\n\r\x0b\x0c"

typedef long long llong;
typedef unsigned long ulong;
typedef unsigned long long ullong;

Error parse_long(char **str, long *value);
Error parse_llong(char **str, llong *value);
Error parse_ulong(char **str, ulong *value);
Error parse_ullong(char **str, ullong *value);
Error parse_size_t(char **str, size_t *value);
