#include <stdlib.h>

#include "error.h"

typedef int (*comparator)(const void *, const void *);

error_t merge_sort(void *ptr, size_t length, size_t size, comparator compare);
