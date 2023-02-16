#include "merge_sort.h"
#include "queue.h"

typedef struct {
    char *name;
    size_t arrival_time;
    size_t service_time;
} Passenger;

typedef struct {
    Queue *queues;
    size_t queue_count;
} LoadBalancer;

error_t default_load_balancer(LoadBalancer *lb, size_t queue_count) {
    lb->queue_count = queue_count;
    NEW(lb->queues, queue_count * sizeof(Queue));
    for (size_t i = 0; i < queue_count; i++) {
        default_queue(lb->queues + i);
    }
    return 0;
}

// parses passenger in a format "name/arrival_time/service_time"
Passenger parse_passenger(const char *string) {
    Passenger passenger = {0};

    char *first = string + strcspn(string, "/");
    char *second = first + strcspn(first, "/");

    NEW(passenger.name, (first - string + 1) * sizeof(char));
    passenger.name[first - string] = '\0';
    if (first) passenger.arrival_time = strtoul(first + 1, second, 10);
    if (second) passenger.service_time = strtoul(second + 1, NULL, 10);

    return passenger;
}


// // parse passenger that looks like that: name/arrival_time/service_time
// // and add it to the queue chosen by the choose function
// error_t parse_passenger(LoadBalancer *lb, char *passenger,
//                         Queue (*choose)(LoadBalancer *lb)) {
//     char *name = passenger;
//     char *arrival_time = strchr(passenger, '/');
//     if (arrival_time == NULL) {
//         return 1;
//     }
//     *arrival_time = '\0';
//     arrival_time++;
//     char *service_time = strchr(arrival_time, '/');
//     if (service_time == NULL) {
//         return 1;
//     }
//     *service_time = '\0';
//     service_time++;
//     Passenger p = {name, atoi(arrival_time), atoi(service_time)};
//     size_t queue_index = choose(lb);
//     return queue_push(lb->queues[queue_index], p);
// }

// Passenger parse_passenger(char *string) {
//     char *name = string;
//     char *arrival_time = strchr(string, '/');
//     if (arrival_time == NULL) {
//         return 1;
//     }
//     *arrival_time = '\0';
//     arrival_time++;
//     char *service_time = strchr(arrival_time, '/');
//     if (service_time == NULL) {
//         return 1;
//     }
//     *service_time = '\0';
//     service_time++;

//     return (Passenger){.name = name,
//                        .arrival_time = atoi(arrival_time),
//                        .service_time = atoi(service_time)};
// }

// score queue by the strategy "Least Time":
// first, choose the queue with the least amount of passengers
// second, choose the queue with the smaller average service time
size_t score_queue_least_time(LoadBalancer *lb, size_t queue_index) {
    size_t queue_size = lb->queues[queue_index].size;
    size_t queue_time = 0;
    for (size_t i = 0; i < queue_size; i++) {
        queue_time += lb->queues[queue_index].data[i].service_time;
    }
    return queue_size + queue_time / queue_size;
}

int size_t_cmp(const void *a, const void *b) {
    size_t l = *(size_t *)a;
    size_t r = *(size_t *)b;
    return (l > r) - (l < r);
}

// choose the queue using the strategy "Least Time":
// first, choose the queue with the least amount of passengers
// second, choose the queue with the smaller average service time
Queue choose_least_time(LoadBalancer *lb) {
    // applying stable sorts in reverse order
    // is the same as sorting based on multiple criteria

    Queue *copy = NEW(copy, lb->queue_count * sizeof(Queue));
    memcpy(copy, lb->queues, lb->queue_count * sizeof(Queue));

    qsort(people, 20, sizeof(Person), comparators[2]);
    qsort(people, 20, sizeof(Person), comparators[1]);
}

Queue choose_least_time(LoadBalancer *lb) {
    // applying stable sorts in reverse order
    // is the same as sorting based on multiple criteria

    Queue *queues = NEW(queues, lb->queue_count * sizeof(Queue));
    memcpy(queues, lb->queues, lb->queue_count * sizeof(Queue));

    // first, choose the queue with the least amount of passengers
    merge_sort(queues, lb->queue_count, sizeof(Queue), size_t_cmp);

    // second, choose the queue with the smaller average service time
    // smaller average service time means more passengers served
    qsort(people, 20, sizeof(Person), comparators[1]);
}

// int queue_least_time_compare(const void *a, const void *b) {
//     Queue *l = (Queue *)a;
//     Queue *r = (Queue *)b;
//     size_t l_size = l->size;
//     size_t r_size = r->size;
//     size_t l_time = 0;
//     size_t r_time = 0;
//     for (size_t i = 0; i < l_size; i++) {
//         l_time += l->data[i].service_time;
//     }
//     for (size_t i = 0; i < r_size; i++) {
//         r_time += r->data[i].service_time;
//     }
//     return (l_size + l_time / l_size) - (r_size + r_time / r_size);
// }
