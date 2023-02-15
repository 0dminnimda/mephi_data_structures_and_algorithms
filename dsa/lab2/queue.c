#if !defined(QUEUE_IMPL) || (QUEUE_IMPL == 'L')
#include "queue_list_impl.c"
#elif (QUEUE_IMPL == 'V')
#include "queue_vector_impl.c"
#else
#error Unknown QUEUE_IMPL value, should be 'V' or 'L'
#endif
