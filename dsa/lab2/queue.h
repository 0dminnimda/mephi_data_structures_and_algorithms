#ifndef QUEUE_H_
#define QUEUE_H_

#include "error.h"
#include "queue_decl.h"

#define default_queue(queue) AUTO_TRY(default_queue(queue))
#define queue_push(queue, value) AUTO_TRY(queue_push(queue, value))

#endif  // QUEUE_H_
