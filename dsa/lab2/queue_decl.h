#include <stdlib.h>

#include "error.h"
#include "passenger.h"

#ifndef QUEUE_ITEM
#define QUEUE_ITEM Passenger
// #   error You should define 'QUEUE_ITEM' for Queue to work
#endif

#ifndef QUEUE_ITEM_DESTRUCTOR
#define QUEUE_ITEM_DESTRUCTOR(item)
#endif

typedef struct QueueImpl *Queue;

error_t default_queue(Queue *queue);
void destroy_queue(Queue queue);

void queue_clear(Queue queue);

error_t queue_push(Queue queue, QUEUE_ITEM value);
void queue_pop(Queue queue);

QUEUE_ITEM queue_front(Queue queue);
QUEUE_ITEM queue_back(Queue queue);

size_t queue_size(Queue queue);
