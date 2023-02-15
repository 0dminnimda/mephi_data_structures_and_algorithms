#include <stdlib.h>
#include "error.h"

#ifndef QUEUE_ITEM
#   define QUEUE_ITEM int
// #   error You should define 'QUEUE_ITEM' for Queue to work
#endif

#ifndef QUEUE_ITEM_DESTRUCTOR
#   define QUEUE_ITEM_DESTRUCTOR(item)
#endif

typedef struct Queue Queue;

error_t default_queue(Queue **);
void destroy_queue(Queue *);

void queue_clear(Queue *);

error_t queue_push(Queue *, QUEUE_ITEM);
void queue_pop(Queue *);

QUEUE_ITEM queue_front(Queue *);
QUEUE_ITEM queue_back(Queue *);

size_t queue_size(Queue *);
