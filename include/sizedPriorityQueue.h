#ifndef SIZED_PRIORITY_QUEUE_H
#define SIZED_PRIORITY_QUEUE_H
#include "avlTree.h"

typedef struct {
	avlNode *root;
	avlNode *maxNode;
	size_t maxSize;
	size_t currentSize;
	int (*cmp)(const void *, const void *);
	void (*freeData)(void *);
} sizedPriorityQueue;

sizedPriorityQueue newSizedPriorityQueue(const size_t maxSize,
										 int (*cmp)(const void *, const void *),
										 void (*freeData)(void *));
void sizedPriorityQueuePush(sizedPriorityQueue *q, void *data);
void *sizedPriorityQueuePop(sizedPriorityQueue *q);
const void *sizedPriorityQueuePeek(const sizedPriorityQueue *q);
size_t sizedPriorityQueueGetSize(const sizedPriorityQueue *q);
bool sizedPriorityQueueIsEmpty(const sizedPriorityQueue *q);
void sizedPriorityQueueClear(sizedPriorityQueue *q);
void sizedPriorityQueueFree(sizedPriorityQueue *q);




#endif
