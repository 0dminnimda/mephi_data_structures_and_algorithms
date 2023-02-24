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

Error default_queue(Queue *queue) {
    NEW(*queue, sizeof(struct QueueImpl));
    (*queue)->head = NULL;
    (*queue)->tail = NULL;
    (*queue)->size = 0;
    return OK;
}

Error construct_queue(Queue *queue, size_t size) {
    AUTO_TRY(default_queue(queue));
    return OK;
}

void queue_clear(Queue queue) {
    while (queue->size > 0) queue_pop(queue);
    assert(queue->head == NULL);
}

void destroy_queue(Queue queue) {
    if (queue) queue_clear(queue);
    free(queue);
}

Error queue_push(Queue queue, QUEUE_ITEM value) {
    QueueNode *node = NEW(node, sizeof(QueueNode));
    node->value = value;
    node->next = NULL;
    if (queue->size == 0)
        queue->head = node;
    else
        queue->tail->next = node;
    queue->tail = node;
    queue->size++;
    return OK;
}

Error queue_pop(Queue queue) {
    if (queue->size <= 0)
        return QUEUE_ERROR("U stupid, queue is empty");
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
