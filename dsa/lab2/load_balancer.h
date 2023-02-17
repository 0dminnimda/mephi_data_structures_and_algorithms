#ifndef LOAD_BALANCER_H_
#define LOAD_BALANCER_H_

#include <stdlib.h>

#include "passenger.h"
#include "queue.h"

typedef struct {
    Queue queue;
    size_t served;
    size_t uptime;
} PassengerQueue;

typedef struct {
    PassengerQueue *queues;
    size_t queue_count;
} LoadBalancer;

error_t construct_load_balancer(LoadBalancer *lb, size_t queue_count);
void destroy_load_balancer(LoadBalancer *lb);

typedef int (*queue_comparator)(const PassengerQueue *a,
                                const PassengerQueue *b);

size_t choose_queue(LoadBalancer *lb, queue_comparator queue_cmp);
size_t choose_least_time_queue(LoadBalancer *lb);

#endif  // LOAD_BALANCER_H_
