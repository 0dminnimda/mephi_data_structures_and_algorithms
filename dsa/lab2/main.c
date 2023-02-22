#ifndef TEST

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "input.h"
#include "load_balancer.h"
#include "queue.h"
#include "readline.h"

Error sub_main() {
    int queue_count;
    if (get_non_negative_int("", &queue_count))
        return MAKE_ERROR("InputError", "Not a non-negative integer");

    LoadBalancer lb;
    TRY(construct_load_balancer(&lb, queue_count));

    char *input = readline(NULL);

    printf("Hello, World!\n");

    destroy_load_balancer(&lb);

    return OK;
}

int main() {
    THROW_ERRORS(sub_main());
    return 0;
}

#else  // TEST
#include "test.c"
#endif  // TEST
