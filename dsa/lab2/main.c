#ifndef TEST

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "input.h"
#include "load_balancer.h"
#include "memo.h"
#include "passenger.h"
#include "queue.h"
#include "string.h"

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
        Passenger passenger = {0};
        TRY(parse_passenger(&cur, &passenger))
        CATCH_ALL {
            // printf("\n%s\n", line);
            for (size_t i = 0; i < cur - line; i++) printf(" ");
            printf("^\n");
            for (size_t i = 0; i < cur - line; i++) printf(" ");
            printf("The wrong part\n");
            break;
        }

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
