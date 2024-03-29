#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>

#include "../sugar/sugar.h"
#include "../passenger.h"

#ifndef QUEUE_ITEM
#define QUEUE_ITEM Passenger
// #   error You should define 'QUEUE_ITEM' for Queue to work
#endif

#ifndef QUEUE_ITEM_DESTRUCTOR
#define QUEUE_ITEM_DESTRUCTOR(item) destroy_passenger(item)
// #define QUEUE_ITEM_DESTRUCTOR(item)
#endif

#ifndef QUEUE_ITEM_FPRINT
#define QUEUE_ITEM_FPRINT(stream, item) fprint_passenger_name(stream, item)
// #define QUEUE_ITEM_FPRINT(stream, item)
#endif

typedef struct QueueImpl *Queue;

#define QUEUE_ERROR_TYPE MAKE_ERROR_TYPE("QueueError")
#define QUEUE_ERROR(message) MAKE_ERROR(QUEUE_ERROR_TYPE, message)

Error default_queue(Queue *queue);
Error construct_queue(Queue *queue, size_t size);
void destroy_queue(Queue queue);

size_t queue_size(Queue queue);
void queue_clear(Queue queue);

Error queue_front(Queue queue, QUEUE_ITEM *value);
Error queue_back(Queue queue, QUEUE_ITEM *value);

Error queue_push(Queue queue, QUEUE_ITEM value);
Error queue_pop(Queue queue);

void fprint_queue(FILE *stream, Queue queue);
void print_queue(Queue queue);

#endif  // QUEUE_H_
