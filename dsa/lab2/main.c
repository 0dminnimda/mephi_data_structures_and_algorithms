#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

int main() {
    Queue q;
    default_queue(&q);

#if FAILURE_TEST == 1
    queue_pop(q);
#elif FAILURE_TEST == 2
    queue_front(q);
#elif FAILURE_TEST == 3
    queue_back(q);
#endif

    while (1) {
        int x;
        scanf("%d", &x);
        if (x == 0) break;
        queue_push(q, x);
    }

    while (queue_size(q)) {
        printf("%d ", queue_front(q));
        queue_pop(q);
    }
    printf("\n");

    while (1) {
        int x;
        scanf("%d", &x);
        if (x == 0) break;
        queue_push(q, x);
    }

    if (queue_size(q))
        printf("%d %d\n", queue_front(q), queue_back(q));
    else
        printf("empty\n");

    queue_clear(q);

    while (queue_size(q)) {
        printf("%d ", queue_front(q));
        queue_pop(q);
    }
    printf("\n");

    destroy_queue(q);
}
