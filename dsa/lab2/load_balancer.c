#include "load_balancer.h"

#include <assert.h>

#include "memo.h"
#include "merge_sort.h"

error_t construct_load_balancer(LoadBalancer *lb, size_t queue_count) {
    lb->queue_count = queue_count;
    NEW(lb->queues, queue_count * sizeof(PassengerQueue));
    for (size_t i = 0; i < queue_count; i++) {
        default_queue(&lb->queues[i].queue);
        lb->queues[i].served = 0;
        lb->queues[i].uptime = 0;
    }
    return 0;
}

void destroy_load_balancer(LoadBalancer *lb) {
    if (lb == NULL) return;
    for (size_t i = 0; i < lb->queue_count; i++)
        destroy_queue(lb->queues[i].queue);
    free(lb->queues);
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

    // second, choose the queue with the smaller average service time
    if (l->uptime / l->served < r->uptime / r->served) return -1;
    if (l->uptime / l->served > r->uptime / r->served) return 1;

    return 0;
}

size_t choose_least_time_queue(LoadBalancer *lb) {
    return choose_queue(lb, least_time_queue_cmp);
}
