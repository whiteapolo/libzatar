#include <string.h>
#include "queue.h"

queue newQueue()
{
	queue q;
	q.start = NULL;
	q.end = NULL;
	return q;
}

void enqueue(queue *q, void *data)
{
	if (q->start == NULL) {
		listPush(&q->start, data);
		q->end = q->start;
	} else {
		listInsertAfter(q->end, data);
		q->end = q->end->next;
	}
}

void *dequeue(queue *q)
{
	list *tmp = q->start->next;
	void *data = listPop(&q->start);
	q->start = tmp;
	return data;
}

bool queueIsEmpty(const queue *q)
{
	return q->start == NULL;
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
			freeData(dequeue(q));
}
