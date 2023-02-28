#ifndef LOAD_BALANCER_H_
#define LOAD_BALANCER_H_

#include <stdlib.h>

#include "sugar/sugar.h"
#include "passenger.h"
#include "queue/queue.h"

typedef struct {
    Queue queue;
    size_t served;
    size_t serviced_time;
    size_t service_time_left;
} PassengerQueue;

typedef struct {
    PassengerQueue *queues;
    size_t queue_count;
} LoadBalancer;

Error construct_load_balancer(LoadBalancer *lb, size_t queue_count);
void destroy_load_balancer(LoadBalancer *lb);

Error load_balancer_push(LoadBalancer *lb, size_t i, Passenger passenger);
Error load_balancer_update(LoadBalancer *lb, size_t delta_time);

typedef int (*queue_comparator)(const PassengerQueue *a,
                                const PassengerQueue *b);

size_t choose_queue(LoadBalancer *lb, queue_comparator queue_cmp);
size_t choose_least_time_queue(LoadBalancer *lb);

#endif  // LOAD_BALANCER_H_
