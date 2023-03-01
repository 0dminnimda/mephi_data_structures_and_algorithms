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

void point_on_error(char *current, char *line) {
    printf("\n%s\n", line);
    for (size_t i = 0; i < current - line; i++) printf(" ");
    printf("^\n");
    for (size_t i = 0; i < current - line; i++) printf(" ");
    printf("The wrong part\n");
}


Error sub_main() {
    Error error = OK;

    printf("Input passengers\n");
    char *line = read_line();
    char *cur = line;

    size_t queue_count = 0;
    TRY(parse_size_t(&cur, &queue_count))
    CATCH(PARSE_ERROR_TYPE) {
        point_on_error(cur, line);
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
            point_on_error(cur, line);
            break;
        }
        CATCH_ALL break;

        if (prev_arrival_time > passenger.arrival_time) {
            point_on_error(pre_cur, line);
            error = PARSE_ERROR(
                "Passengers should be introduced"
                " in order of their arrival_time");
            break;
        } else if (prev_arrival_time != passenger.arrival_time) {
            AUTO_TRY(load_balancer_update(
                &lb, passenger.arrival_time - prev_arrival_time));
            printf("Time %zu\n", passenger.arrival_time);
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

    if (!IS_ERROR(error)) {
        while (1) {
            size_t delta_time = load_balancer_min_nonzero_delta_time(&lb);
            if (delta_time == 0) break;
            prev_arrival_time += delta_time;
            printf("Time %zu\n", prev_arrival_time);
            AUTO_TRY(load_balancer_update(&lb, delta_time));
            AUTO_TRY(print_load_balancer(&lb));
        }
    }

    DELETE(line);
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

/*
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9
2 a/1/20 b/1/15 c/2/ d/5/8 e/6/5 f/6/9
2 a/1/20 b/1/15 c/2 d/5/8 e/6/5 f/6/9
2 a/1/20 b/1/15 c/ d/5/8 e/6/5 f/6/9
2 a/1/20 b/1/15 c d/5/8 e/6/5 f/6/9
2 a/1/20 b/1/15  d/5/8 e/6/5 f/6/9
2 a/1/20 b/1/15 c//10 d/5/8 e/6/5 f/6/9
2 a/1/20 b/1/15 /2/10 d/5/8 e/6/5 f/6/9

2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f//9
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 /6/9

2 a/0/20
2 a/1/0

2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9 g/20/3 h/20/5 i/21/4 j/25/9 i/25/3 k/25/7 l/100/10 m/111/9 n/112/3 o/112/10
*/

/*
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/ d/5/8 e/6/5 f/6/9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2 d/5/8 e/6/5 f/6/9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/ d/5/8 e/6/5 f/6/9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c d/5/8 e/6/5 f/6/9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15  d/5/8 e/6/5 f/6/9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c//10 d/5/8 e/6/5 f/6/9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 /2/10 d/5/8 e/6/5 f/6/9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f//9
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 /6/9
valgrind --leak-check=full ./main.out
2 a/0/20
valgrind --leak-check=full ./main.out
2 a/1/0
valgrind --leak-check=full ./main.out
2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9 g/20/3 h/20/5 i/21/4 j/25/9 i/25/3 k/25/7 l/100/10 m/111/9 n/112/3 o/112/10
*/
