#ifndef TEST

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "queue.h"
#include "load_balancer.h"

int main() {
    size_t queue_count;
    scanf("%zu", &queue_count);

    LoadBalancer lb;
    WITH_ERROR(construct_load_balancer(&lb, queue_count));

    printf("Hello, World!\n");

    destroy_load_balancer(&lb);
}

#else  // TEST
#include "test.c"
#endif  // TEST
