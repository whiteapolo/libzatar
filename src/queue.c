#include <string.h>
#include "queue.h"

queue newQueue()
{
	queue q;
	q.start = NULL;
	q.end = NULL;
	q.size = 0;
	return q;
}

void queuePush(queue *q, void *data)
{
	q->size++;
	if (q->start == NULL) {
		listPush(&q->start, data);
		q->end = q->start;
	} else {
		listInsertAfter(q->end, data);
		q->end = q->end->next;
	}
}

void *queuePop(queue *q)
{
	q->size--;
	list *tmp = q->start->next;
	void *data = listPop(&q->start);
	q->start = tmp;
	return data;
}

void *queuePeek(queue *q)
{
	return q->start->data;
}

bool queueIsEmpty(const queue *q)
{
	return q->start == NULL;
}

size_t queueGetSize(const queue *q)
{
	return q->size;
}

void queuePrint(const queue *q, void (*printData)(void *))
{
	listPrint(q->start, printData);
}

void queueClear(queue *q, void (*freeData)(void *))
{
	queueFree(q, freeData);
	*q = newQueue();
}

void queueFree(queue *q, void (*freeData)(void *))
{
	if (freeData)
		while (!queueIsEmpty(q))
			freeData(queuePop(q));
}
