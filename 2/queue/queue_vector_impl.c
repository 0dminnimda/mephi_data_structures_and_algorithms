#include <assert.h>
#include <string.h>

#include "queue.h"

struct QueueImpl {
    QUEUE_ITEM *data;
    size_t capacity;
    size_t size;
    size_t head;
};

static inline size_t real_index(Queue queue, size_t index) {
    return (queue->head + index) % queue->capacity;
}

static inline QUEUE_ITEM *at(Queue queue, size_t index) {
    return &queue->data[real_index(queue, index)];
}

static void free_from(Queue queue, size_t index) {
    for (size_t i = index; i < queue->size; i++)
        QUEUE_ITEM_DESTRUCTOR(*at(queue, i));
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
    if (queue) DELETE(queue->data);
    DELETE(queue);
}

Error queue_push(Queue queue, QUEUE_ITEM value) {
    if (queue->capacity <= queue->size) return QUEUE_ERROR("Ran out of memory");
    *at(queue, queue->size) = value;
    queue->size++;
    return OK;
}

Error queue_pop(Queue queue) {
    if (queue->size <= 0) return QUEUE_ERROR("U stupid, queue is empty");
    QUEUE_ITEM_DESTRUCTOR(*at(queue, 0));
    queue->head++;
    queue->head = queue->head % queue->capacity;
    queue->size--;
    return OK;
}

Error queue_front(Queue queue, QUEUE_ITEM *value) {
    if (queue->size <= 0) return QUEUE_ERROR("U stupid, queue is empty");
    *value = *at(queue, 0);
    return OK;
}

Error queue_back(Queue queue, QUEUE_ITEM *value) {
    if (queue->size <= 0) return QUEUE_ERROR("U stupid, queue is empty");
    *value = *at(queue, queue->size - 1);
    return OK;
}

size_t queue_size(Queue queue) { return queue->size; }

void fprint_queue(FILE *stream, Queue queue) {
    for (size_t i = 0; i < queue->size; i++) {
        if (i) fprintf(stream, ", ");
        QUEUE_ITEM_FPRINT(stream, at(queue, i));
    }
}

void print_queue(Queue queue) { fprint_queue(stdout, queue); }
