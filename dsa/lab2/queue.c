#if QUEUE_IMP == 'L'
#   include "queue_list_impl.c"
#else
#   include "queue_vector_impl.c"
#endif
