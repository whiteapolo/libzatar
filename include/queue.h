#ifndef QUEUE_H
#define QUEUE_H
#include "list.h"

typedef struct {
    list *start;
    list *end;
} queue;

queue newQueue();
void enqueue(queue *q, void *data);
void *dequeue(queue *q);
bool queueIsEmpty(const queue *q);
void queueFree(queue *q, void (*freeData)(void *));
void queuePrint(const queue *q, void (*printData)(void *));

#endif
