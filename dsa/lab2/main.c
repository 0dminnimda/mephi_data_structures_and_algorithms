#ifndef TEST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "input.h"
#include "load_balancer.h"
#include "memo.h"
#include "passenger.h"
#include "queue.h"
#include "string.h"

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

    ulong queue_count = 0;
    TRY(parse_ulong(&cur, &queue_count))
    CATCH(PARSE_ERROR_TYPE) {
        return PARSE_ERROR(
            "Parsed queue count is not a valid non-negative number");
    }
    CATCH_N_THROW

    LoadBalancer lb = {0};
    AUTO_TRY(construct_load_balancer(&lb, queue_count));

    while (1) {
        cur += strspn(cur, " ");
        if (*cur == '\0') break;

        Passenger passenger = {0};
        TRY(parse_passenger(&cur, &passenger))
        CATCH(PARSE_ERROR_TYPE) {
            point_on_error(cur, line);
            break;
        }
        CATCH_ALL break;

        fprint_passenger(stdout, &passenger), printf("\n");

        // size_t i = choose_least_time_queue(&lb);
        // queue_push(lb.queues[i].queue, passenger);
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
*/
