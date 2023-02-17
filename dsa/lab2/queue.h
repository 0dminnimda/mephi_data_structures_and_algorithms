#ifndef QUEUE_H_
#define QUEUE_H_

#include "error.h"
#include "queue_decl.h"

#define default_queue(queue) WITH_ERROR(default_queue(queue))
#define queue_push(queue, value) WITH_ERROR(queue_push(queue, value))

#endif  // QUEUE_H_
