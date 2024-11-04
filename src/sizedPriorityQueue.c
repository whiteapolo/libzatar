#include "sizedPriorityQueue.h"

sizedPriorityQueue newSizedPriorityQueue(const size_t maxSize,
										 int (*cmp)(const void *, const void *),
										 void (*freeData)(void *))
{
	sizedPriorityQueue q;
	q.maxSize = maxSize;
	q.currentSize = 0;
	q.cmp = cmp;
	q.root = NULL;
	q.maxNode = NULL;
	q.freeData = freeData;
	return q;
}

void sizedPriorityQueuePush(sizedPriorityQueue *q, void *data)
{
	avlInsert(&q->root, data, NULL, q->cmp);
	if (q->currentSize + 1 > q->maxSize)
		avlRemove(&q->root, avlGetMax(q->root)->key, q->cmp, q->freeData, NULL);
	else
		q->currentSize++;
	q->maxNode = avlGetMin(q->root);
}

const void *sizedPriorityQueuePeek(const sizedPriorityQueue *q)
{
	return q->maxNode->key;
}

void *sizedPriorityQueuePop(sizedPriorityQueue *q)
{
	q->currentSize--;
	void *data = q->maxNode->key;
	avlRemove(&q->root, data, q->cmp, NULL, NULL);
	if (q->root != NULL)
		q->maxNode = avlGetMin(q->root);
	else
		q->maxNode = NULL;
	return data;
}

size_t sizedPriorityQueueGetSize(const sizedPriorityQueue *q)
{
	return q->currentSize;
}

bool sizedPriorityQueueIsEmpty(const sizedPriorityQueue *q)
{
	return q->currentSize == 0;
}

void sizedPriorityQueueClear(sizedPriorityQueue *q)
{
	avlFree(q->root, q->freeData, NULL);
	*q = newSizedPriorityQueue(q->maxSize, q->cmp, q->freeData);
}

void sizedPriorityQueueFree(sizedPriorityQueue *q)
{
	avlFree(q->root, q->freeData, NULL);
}
