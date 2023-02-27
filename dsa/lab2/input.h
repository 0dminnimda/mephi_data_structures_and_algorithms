#include <stdio.h>

#include "sugar/sugar.h"

#define INPUT_ERROR_TYPE MAKE_ERROR_TYPE("InputError")
#define INPUT_ERROR(message) MAKE_ERROR(INPUT_ERROR_TYPE, message)

char *fread_line(FILE *stream);
char *read_line();
