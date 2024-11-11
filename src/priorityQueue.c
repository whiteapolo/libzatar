#include <stdlib.h>
#include <limits.h>
#include "priorityQueue.h"

priorityQueue newPriorityQueueWithCapacity(int (*cmp)(const void *, const void *), const size_t initializeCapacity)
{
	return newHeapWithCapacity(cmp, initializeCapacity);
}

priorityQueue newPriorityQueue(int (*cmp)(const void *, const void *))
{
	return newHeapWithCapacity(cmp, MIN_PRIORITY_QUEUE_CAPACITY);
}

void priorityQueuePush(priorityQueue *q, void *data)
{
	heapPush(q, data);
}

void *priorityQueuePop(priorityQueue *q)
{
	return heapPop(q);
}

const void *priorityQueuePeek(const priorityQueue *q)
{
	return heapPeek(q);
}

size_t priorityQueueGetSize(const priorityQueue *q)
{
	return heapGetSize(q);
}

bool priorityQueueIsEmpty(const priorityQueue *q)
{
	return heapIsEmpty(q);
}

void priorityQueueShrinkToFit(priorityQueue *q)
{
	heapShrinkToFit(q);
}

void priorityQueueClear(priorityQueue *q, void (*freeData)(void *))
{
	heapClear(q, freeData);
}

void priorityQueueFree(priorityQueue *q, void (*freeData)(void *))
{
	heapFree(q, freeData);
}
