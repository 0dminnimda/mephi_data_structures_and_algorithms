#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "queue.h"

int main() {
    Queue *q = NULL;
    default_queue(&q);

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

    destroy_queue(q);
}
