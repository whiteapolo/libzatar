#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "shared.h"
#include "heap.h"

#define MIN_PRIORITY_QUEUE_CAPACITY 16

typedef heap priorityQueue;

priorityQueue newPriorityQueue(int (*cmp)(const void *, const void *));
priorityQueue newPriorityQueueWithCapacity(int (*cmp)(const void *, const void *), const size_t initializeCapacity);
void priorityQueuePush(priorityQueue *q, void *data);
void *priorityQueuePop(priorityQueue *q);
const void *priorityQueuePeek(const priorityQueue *q);
size_t priorityQueueGetSize(const priorityQueue *q);
bool priorityQueueIsEmpty(const priorityQueue *q);
void priorityQueueShrinkToFit(priorityQueue *q);
void priorityQueueClear(priorityQueue *q, void (*freeData)(void *));
void priorityQueueFree(priorityQueue *q, void (*freeData)(void *));

#endif
