#ifndef LOAD_BALANCER_H_
#define LOAD_BALANCER_H_

#include <stdlib.h>

#include "passenger.h"
#include "queue/queue.h"
#include "sugar/sugar.h"

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

Error construct_load_balancer(LoadBalancer *lb, size_t queue_count,
                              size_t queue_size);
void destroy_load_balancer(LoadBalancer *lb);

Error load_balancer_pop(LoadBalancer *lb, size_t i);
Error load_balancer_push(LoadBalancer *lb, size_t i, Passenger passenger);

Error load_balancer_update(LoadBalancer *lb, size_t delta_time);
Error print_load_balancer(LoadBalancer *lb);
size_t load_balancer_min_nonzero_delta_time(LoadBalancer *lb);

typedef int (*queue_comparator)(const PassengerQueue *a,
                                const PassengerQueue *b);

size_t choose_queue(LoadBalancer *lb, queue_comparator queue_cmp);
size_t choose_least_time_queue(LoadBalancer *lb);

#endif  // LOAD_BALANCER_H_
