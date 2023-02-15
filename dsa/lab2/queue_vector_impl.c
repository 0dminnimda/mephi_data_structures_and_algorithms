#include <string.h>

#include "queue.h"
#include "memo.h"


struct Queue {
    QUEUE_ITEM *data;
    size_t capacity;
    size_t size;
    size_t head;
};

error_t default_queue(Queue **queue) {
    NEW(*queue, sizeof(Queue));
    (*queue)->data = NULL;
    (*queue)->capacity = 0;
    (*queue)->size = 0;
    (*queue)->head = 0;
    return 0;
}

static void clear_from(Queue *queue, size_t index) {
    for (size_t i = index; i < queue->size; i++)
        QUEUE_ITEM_DESTRUCTOR(queue->data[head + i]);
    if (queue->size > index) queue->size = index;
}

void queue_clear(Queue *queue) {
    clear_from(queue, 0);
}

void destroy_queue(Queue *queue) {
    if (queue && queue->data) queue_clear(queue);
}

static void reshift(Queue *queue) {
    memmove(queue->data, queue->data + queue->head, queue->size * sizeof(QUEUE_ITEM));
    queue->head = 0;
}

static error_t resize(Queue *queue, size_t capacity) {
    if (capacity == queue->capacity) return 0;

    clear_from(queue, capacity);
    RENEW(queue->data, queue->capacity * sizeof(QUEUE_ITEM), capacity * sizeof(QUEUE_ITEM));
    queue->capacity = capacity;

    return 0;
}

static error_t reserve(Queue *queue, size_t capacity) {
    if (capacity <= queue->capacity) return 0;
    return resize(queue, capacity);
}

error_t queue_push(Queue *queue, QUEUE_ITEM value) {
    if (queue->capacity <= queue->head + queue->size) {
        if (queue->head)
            reshift(queue);
        else
            WITH_ERROR(reserve(queue, queue->size * 2 + 1));
    }

    queue->data[queue->head + queue->size] = value;
    queue->size++;

    return 0;
}

void queue_pop(Queue *queue) {
    QUEUE_ITEM_DESTRUCTOR(queue->data[queue->head]);
    queue->head++;
    queue->size--;
}

QUEUE_ITEM queue_front(Queue *queue) {
    return queue->data[queue->head];
}

QUEUE_ITEM queue_back(Queue *queue) {
    return queue->data[queue->head + queue->size];
}

size_t queue_size(Queue *queue) {
    return queue->size;
}
