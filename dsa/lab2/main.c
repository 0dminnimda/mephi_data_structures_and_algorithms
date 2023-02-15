#ifndef TEST

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

int main() {
    Queue q;
    default_queue(&q);

    printf("Hello, World!\n");

    destroy_queue(q);
}

#else  // TEST
#include "test.c"
#endif  // TEST
