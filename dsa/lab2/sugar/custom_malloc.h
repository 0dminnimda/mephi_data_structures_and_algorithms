#include <stddef.h>

#if !(defined(__STDC__) && __STDC_VERSION__ >= 201112L)
typedef long double max_align_t;
#endif

void *custom_malloc(size_t size);
void *custom_calloc(size_t num, size_t size);
void *custom_realloc(void *ptr, size_t size);
size_t custom_malloc_size(const void *ptr);
void custom_free(void *ptr);

#ifdef CUSTOM_MALLOC_NO_DEFINES
#define malloc custom_malloc
#define calloc custom_calloc
#define realloc custom_realloc
#define malloc_size custom_malloc_size
#define free custom_free
#endif
