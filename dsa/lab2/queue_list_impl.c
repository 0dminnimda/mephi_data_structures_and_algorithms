#include <assert.h>

#include "memo.h"
#include "queue_decl.h"

typedef struct QueueNode {
    QUEUE_ITEM value;
    struct QueueNode *next;
} QueueNode;

struct QueueImpl {
    QueueNode *head;
    QueueNode *tail;
    size_t size;
};

error_t default_queue(Queue *queue) {
    NEW(*queue, sizeof(struct QueueImpl));
    (*queue)->head = NULL;
    (*queue)->tail = NULL;
    (*queue)->size = 0;
    return 0;
}

void queue_clear(Queue queue) {
    while (queue->size > 0) queue_pop(queue);
    assert(queue->head == NULL);
}

void destroy_queue(Queue queue) {
    if (queue) queue_clear(queue);
    free(queue);
}

error_t queue_push(Queue queue, QUEUE_ITEM value) {
    QueueNode *node = NEW(node, sizeof(QueueNode));
    node->value = value;
    node->next = NULL;
    if (queue->size == 0)
        queue->head = node;
    else
        queue->tail->next = node;
    queue->tail = node;
    queue->size++;
    return 0;
}

void queue_pop(Queue queue) {
    assert(queue->size > 0);
    QueueNode *head = queue->head;
    queue->head = queue->head->next;
    QUEUE_ITEM_DESTRUCTOR(head->value);
    free(head);
    queue->size--;
    if (queue->size == 0) queue->tail = NULL;
}

QUEUE_ITEM queue_front(Queue queue) {
    assert(queue->size > 0);
    return queue->head->value;
}

QUEUE_ITEM queue_back(Queue queue) {
    assert(queue->size > 0);
    return queue->tail->value;
}

size_t queue_size(Queue queue) { return queue->size; }
