#include "queue.h"
#include "error.h"

#define default_queue(queue) WITH_ERROR(default_queue(queue))
#define queue_push(queue, value) WITH_ERROR(queue_push(queue, value))
