#include <assert.h>

#include "queue.h"

typedef struct QueueNode {
    QUEUE_ITEM value;
    struct QueueNode *next;
} QueueNode;

struct QueueImpl {
    QueueNode *tail;
    size_t size;
};

Error default_queue(Queue *queue) {
    NEW(*queue, sizeof(struct QueueImpl));
    (*queue)->tail = NULL;
    (*queue)->size = 0;
    return OK;
}

Error construct_queue(Queue *queue, size_t size) {
    return default_queue(queue);
}

void queue_clear(Queue queue) {
    while (queue->size > 0) queue_pop(queue);
    assert(queue->tail == NULL);
}

void destroy_queue(Queue queue) {
    if (queue) queue_clear(queue);
    free(queue);
}

Error queue_push(Queue queue, QUEUE_ITEM value) {
    QueueNode *node = NEW(node, sizeof(QueueNode));
    node->value = value;
    if (queue->size == 0) {
        node->next = node;
    } else {
        node->next = queue->tail->next;
        queue->tail->next = node;
    }
    queue->tail = node;
    queue->size++;
    return OK;
}

Error queue_pop(Queue queue) {
    if (queue->size <= 0) return QUEUE_ERROR("U stupid, queue is empty");
    QueueNode *head = queue->tail->next;
    queue->tail->next = queue->tail->next->next;
    QUEUE_ITEM_DESTRUCTOR(head->value);
    free(head);
    queue->size--;
    if (queue->size == 0) queue->tail = NULL;
    return OK;
}

Error queue_front(Queue queue, QUEUE_ITEM *value) {
    if (queue->size <= 0) return QUEUE_ERROR("U stupid, queue is empty");
    *value = queue->tail->next->value;
    return OK;
}

Error queue_back(Queue queue, QUEUE_ITEM *value) {
    if (queue->size <= 0) return QUEUE_ERROR("U stupid, queue is empty");
    *value = queue->tail->value;
    return OK;
}

size_t queue_size(Queue queue) { return queue->size; }

void fprint_queue(FILE *stream, Queue queue) {
    if (queue->tail == NULL) return;
    QueueNode *node = queue->tail->next;
    for (size_t i = 0; i < queue->size; i++, node = node->next) {
        QUEUE_ITEM_FPRINT(stream, &node->value);
        if (node != queue->tail) fprintf(stream, ", ");
    }
}

void print_queue(Queue queue) { fprint_queue(stdout, queue); }
