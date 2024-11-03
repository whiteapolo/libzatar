#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "shared.h"

typedef struct {
	void **vec;
	int size;
	int capacity;
	int (*cmp)(const void *, const void *);
} heap;

typedef heap priorityQueue;

priorityQueue newPriorityQueue(int (*cmp)(const void *, const void *));
void priorityQueuePush(priorityQueue *q, void *data);
void *priorityQueuePop(priorityQueue *q);
const void *priorityQueuePeek(const priorityQueue *q);
int priorityQueueGetSize(const priorityQueue *q);
bool priorityQueueIsEmpty(const priorityQueue *q);
void priorityQueueClear(priorityQueue *q, void (*freeData)(void *));
void priorityQueueFree(priorityQueue *q, void (*freeData)(void *));

#endif
