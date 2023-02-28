#include <assert.h>
#include <string.h>

#include "queue.h"

struct QueueImpl {
    QUEUE_ITEM *data;
    size_t capacity;
    size_t size;
    size_t head;
};

static void free_from(Queue queue, size_t index) {
    for (size_t i = index; i < queue->size; i++)
        QUEUE_ITEM_DESTRUCTOR(queue->data[queue->head + i]);
    if (queue->size > index) queue->size = index;
}

static void reshift(Queue queue) {
    memmove(queue->data, queue->data + queue->head,
            queue->size * sizeof(QUEUE_ITEM));
    queue->head = 0;
}

static Error resize(Queue queue, size_t capacity) {
    if (capacity == queue->capacity) return OK;

    free_from(queue, capacity);
    RENEW(queue->data, capacity * sizeof(QUEUE_ITEM));
    queue->capacity = capacity;

    return OK;
}

static Error reserve(Queue queue, size_t capacity) {
    if (capacity <= queue->capacity) return OK;
    return resize(queue, capacity);
}

Error default_queue(Queue *queue) {
    NEW(*queue, sizeof(struct QueueImpl));
    (*queue)->data = NULL;
    (*queue)->capacity = 0;
    (*queue)->size = 0;
    (*queue)->head = 0;
    return OK;
}

Error construct_queue(Queue *queue, size_t size) {
    AUTO_TRY(default_queue(queue));
    AUTO_TRY(resize(*queue, size));
    return OK;
}

void queue_clear(Queue queue) {
    free_from(queue, 0);
    queue->head = 0;
}

void destroy_queue(Queue queue) {
    if (queue && queue->data) queue_clear(queue);
    if (queue) free(queue->data);
    free(queue);
}

Error queue_push(Queue queue, QUEUE_ITEM value) {
    if (queue->capacity <= queue->head + queue->size)
        if (queue->head)
            reshift(queue);
        else
            return QUEUE_ERROR("Ran out of memory");

    queue->data[queue->head + queue->size] = value;
    queue->size++;
    return OK;
}

Error queue_pop(Queue queue) {
    if (queue->size <= 0) return QUEUE_ERROR("U stupid, queue is empty");
    QUEUE_ITEM_DESTRUCTOR(queue->data[queue->head]);
    queue->head++;
    queue->size--;
    return OK;
}

QUEUE_ITEM queue_front(Queue queue) {
    assert(queue->size > 0);
    return queue->data[queue->head];
}

QUEUE_ITEM queue_back(Queue queue) {
    assert(queue->size > 0);
    return queue->data[queue->head + queue->size - 1];
}

size_t queue_size(Queue queue) { return queue->size; }

void fprint_queue(FILE *stream, Queue queue) {
    for (size_t i = 0; i < queue->size; i++) {
        QUEUE_ITEM_FPRINT(stream, &queue->data[queue->head + i]);
        if (i) fprintf(stream, ", ");
    }
}

void print_queue(Queue queue) { fprint_queue(stdout, queue); }
