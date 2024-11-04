#include "sizedPriorityQueue.h"

sizedPriorityQueue newSizedPriorityQueue(const size_t maxSize, int (*cmp)(const void *, const void *))
{
	sizedPriorityQueue q;
	q.maxSize = maxSize;
	q.currentSize = 0;
	q.cmp = cmp;
	q.root = NULL;
	return q;
}

void *sizedPriorityQueuePush(sizedPriorityQueue *q, void *data)
{
	avlInsert(&q->root, data, NULL, q->cmp);
	if (q->currentSize + 1 > q->maxSize) {
		avlNode *toRemove = avlGetMin(q->root);
		void *data = toRemove->key;
		avlRemove(&q->root, data, q->cmp, NULL, NULL);
		return data;
	} else {
		q->currentSize++;
		return NULL;
	}
}

const void *sizedPriorityQueuePeek(const sizedPriorityQueue *q)
{
	return avlGetMax(q->root)->key;
}

void *sizedPriorityQueuePop(sizedPriorityQueue *q)
{

}
size_t sizedPriorityQueueGetSize(const sizedPriorityQueue *q);
bool sizedPriorityQueueIsEmpty(const sizedPriorityQueue *q);
void sizedPriorityQueueClear(sizedPriorityQueue *q);
void sizedPriorityQueueFree(sizedPriorityQueue *q);
