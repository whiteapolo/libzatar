#ifndef QUEUE_H
#define QUEUE_H
#include "circularList.h"

typedef struct {
	circularList *cl;
	u64 size;
} Queue;

Queue *newQueue();
void queuePush(Queue *q, void *data);
void *queuePop(Queue *q);
void *queuePeek(Queue *q);
bool queueIsEmpty(const Queue *q);
u64 queueGetSize(const Queue *q);
void queuePrint(const Queue *q, void printData(const void *));
void queueClear(Queue *q, void freeData(void *));
void queueFree(Queue *q, void freeData(void *));
circularList *queueToCircularList(Queue *q);

#endif
