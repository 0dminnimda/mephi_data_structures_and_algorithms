#include "load_balancer.h"

#include <assert.h>

#include "sugar/sugar.h"

#ifndef SIZE_T_MAX
#define SIZE_T_MAX ((size_t)(-1))
#endif

Error construct_load_balancer(LoadBalancer *lb, size_t queue_count,
                              size_t queue_size) {
    lb->queue_count = queue_count;
    NEW(lb->queues, queue_count * sizeof(PassengerQueue));
    for (size_t i = 0; i < queue_count; i++) {
        AUTO_TRY(construct_queue(&lb->queues[i].queue, queue_size));
        lb->queues[i].served = 0;
        lb->queues[i].serviced_time = 0;
        lb->queues[i].service_time_left = 0;
    }
    return OK;
}

void destroy_load_balancer(LoadBalancer *lb) {
    if (lb == NULL) return;
    for (size_t i = 0; i < lb->queue_count; i++)
        destroy_queue(lb->queues[i].queue);
    DELETE(lb->queues);
}

static Error update_queue(LoadBalancer *lb, size_t i, size_t delta_time) {
    PassengerQueue *pq = &lb->queues[i];
    if (queue_size(pq->queue) == 0) {
        assert(pq->service_time_left == 0);
        return OK;
    }
    if (delta_time < pq->service_time_left) {
        pq->serviced_time += delta_time;
        pq->service_time_left -= delta_time;
        return OK;
    }

    if (pq->service_time_left != 0) {
        AUTO_TRY(queue_pop(pq->queue));
        pq->served++;
        pq->serviced_time += pq->service_time_left;
    }
    Passenger p = {0};
    if (queue_size(pq->queue) != 0) {
        AUTO_TRY(queue_front(pq->queue, &p));
    } else {
        pq->service_time_left = 0;
        return OK;
    }
    delta_time -= pq->service_time_left;
    pq->service_time_left = p.service_time;
    return update_queue(lb, i, delta_time);
}

Error load_balancer_update(LoadBalancer *lb, size_t delta_time) {
    for (size_t i = 0; i < lb->queue_count; i++) {
        AUTO_TRY(update_queue(lb, i, delta_time));
    }
    return OK;
}

size_t load_balancer_min_nonzero_delta_time(LoadBalancer *lb) {
    size_t delta_time = SIZE_T_MAX;
    for (size_t i = 0; i < lb->queue_count; i++) {
        size_t value = lb->queues[i].service_time_left;
        if (value < delta_time && value != 0) delta_time = value;
    }
    if (delta_time == SIZE_T_MAX) return 0;
    return delta_time;
}

Error load_balancer_pop(LoadBalancer *lb, size_t i) {
    AUTO_TRY(queue_pop(lb->queues[i].queue));
    return OK;
}

Error load_balancer_push(LoadBalancer *lb, size_t i, Passenger passenger) {
    AUTO_TRY(queue_push(lb->queues[i].queue, passenger));
    return OK;
}

Error print_load_balancer(LoadBalancer *lb) {
    AUTO_TRY(load_balancer_update(lb, 0));  // load up the updates
    for (size_t i = 0; i < lb->queue_count; i++) {
        printf("  queue %zu (%zu|%zu|%zu): ", i, lb->queues[i].served,
               lb->queues[i].serviced_time, lb->queues[i].service_time_left);
        print_queue(lb->queues[i].queue);
        printf("\n");
    }
    printf("\n");
    return OK;
}

size_t choose_queue(LoadBalancer *lb, queue_comparator queue_cmp) {
    assert(lb->queue_count > 0);
    size_t index = 0;
    for (size_t i = 0; i < lb->queue_count; i++) {
        if (queue_cmp(lb->queues + i, lb->queues + index) < 0) index = i;
    }
    return index;
}

// compare two queues on the strategy "Least Time"
static int least_time_queue_cmp(const PassengerQueue *l,
                                const PassengerQueue *r) {
    // first, choose the queue with the least amount of passengers
    if (queue_size(l->queue) < queue_size(r->queue)) return -1;
    if (queue_size(l->queue) > queue_size(r->queue)) return 1;

    size_t l_score = (l->served == 0) ? 0 : l->serviced_time / l->served;
    size_t r_score = (r->served == 0) ? 0 : r->serviced_time / r->served;

    // second, choose the queue with the smaller average service time
    if (l_score < r_score) return -1;
    if (l_score > r_score) return 1;

    return 0;
}

size_t choose_least_time_queue(LoadBalancer *lb) {
    return choose_queue(lb, least_time_queue_cmp);
}
