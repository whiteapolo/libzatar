#ifndef QUEUE_H
#define QUEUE_H
#include "list.h"

typedef struct {
    list *start;
    list *end;
	size_t size;
} queue;

queue newQueue();
void queuePush(queue *q, void *data);
void *queuePop(queue *q);
void *queuePeek(queue *q);
bool queueIsEmpty(const queue *q);
size_t queueGetSize(const queue *q);
void queuePrint(const queue *q, void (*printData)(void *));
void queueClear(queue *q, void (*freeData)(void *));
void queueFree(queue *q, void (*freeData)(void *));

#endif
