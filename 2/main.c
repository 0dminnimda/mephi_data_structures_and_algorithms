#ifndef TEST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/input.h"
#include "common/string.h"
#include "load_balancer.h"
#include "passenger.h"
#include "queue/queue.h"
#include "sugar/sugar.h"

void point_on_error(char *line, char *current, size_t length) {
    printf("\n%s\n", line);
    for (size_t i = 0; i < current - line; i++) printf(" ");
    for (size_t i = 0; i < length; i++) printf("^");
    printf("\n");
    for (size_t i = 0; i < current - line; i++) printf(" ");
    printf("The wrong part\n");
}

Error printed_load_balancer_update(LoadBalancer *lb, size_t arrival_time,
                                   size_t delta_time) {
    size_t end_arrival_time =
        (delta_time == -1) ? -1 : arrival_time + delta_time;
    while (arrival_time < end_arrival_time) {
        delta_time = load_balancer_min_nonzero_delta_time(lb);

        if (arrival_time + delta_time > end_arrival_time) {
            delta_time = end_arrival_time - arrival_time;
        }
        if (delta_time == 0) break;
        arrival_time += delta_time;

        if (arrival_time < end_arrival_time)
            printf("Time %zu (+%zu)\n", arrival_time, delta_time);
        AUTO_TRY(load_balancer_update(lb, delta_time));
        if (arrival_time < end_arrival_time) AUTO_TRY(print_load_balancer(lb));
    }
    return OK;
}

Error sub_main() {
    Error error = OK;

    printf("Input passengers\n");
    char *line = read_line();
    char *cur = line;

    size_t queue_count = 0;
    TRY(parse_size_t(&cur, &queue_count))
    CATCH(PARSE_ERROR_TYPE) {
        point_on_error(line, cur, 1);
        return PARSE_ERROR(
            "Parsed queue count is not a valid non-negative number");
    }
    CATCH_N_THROW

    size_t queues_size = 4;
    LoadBalancer lb = {0};
    AUTO_TRY(construct_load_balancer(&lb, queue_count, queues_size));

    printf("\n");
    printf("Time %zu\n", (size_t)0);
    AUTO_TRY(print_load_balancer(&lb));

    size_t prev_arrival_time = 0;
    while (1) {
        cur += strspn(cur, " ");
        if (*cur == '\0') break;

        char *pre_cur = cur;
        Passenger passenger = {0};
        TRY(parse_passenger(&cur, &passenger))
        CATCH(PARSE_ERROR_TYPE) {
            point_on_error(line, cur, 1);
            break;
        }
        CATCH_ALL break;

        if (prev_arrival_time > passenger.arrival_time) {
            destroy_passenger(passenger);
            point_on_error(line, pre_cur, cur-pre_cur);
            error = PARSE_ERROR(
                "Passengers should be introduced"
                " in order of their arrival_time");
            break;
        } else if (prev_arrival_time != passenger.arrival_time) {
            size_t dt = passenger.arrival_time - prev_arrival_time;
            AUTO_TRY(printed_load_balancer_update(&lb, prev_arrival_time, dt));
            printf("Time %zu (+%zu)\n", passenger.arrival_time, dt);
        }
        prev_arrival_time = passenger.arrival_time;

        size_t i = choose_least_time_queue(&lb);
        TRY(load_balancer_push(&lb, i, passenger))
        CATCH_ALL {
            destroy_passenger(passenger);
            break;
        }
        print_load_balancer(&lb);
    }

    if (!IS_ERROR(error))
        AUTO_TRY(printed_load_balancer_update(&lb, prev_arrival_time, -1));

    free(line);  // since read_line uses regular malloc
    destroy_load_balancer(&lb);

    return error;
}

int main() {
    Error error;
    TRY(sub_main())
    CATCH_ALL {
        FPRINT_VERBOSE_ERROR(stderr, error);
        return -1;
    }
    return 0;
}

#else  // TEST
#include "test.c"
#endif  // TEST
