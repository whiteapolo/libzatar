#include <string.h>
#include "queue.h"
#include "list.h"
#include <stdlib.h>

Queue *newQueue()
{
	Queue *q = malloc(sizeof(Queue));
	q->size = 0;
	q->cl = NULL;

	return q;
}

void queuePush(Queue *q, void *data)
{
	q->size++;
	circularListInsertLast(&q->cl, data);
}

void *queuePop(Queue *q)
{
	q->size--;
	return circularListPop(&q->cl);
}

void *queuePeek(Queue *q)
{
	return q->cl->next->data;
}

bool queueIsEmpty(const Queue *q)
{
	return q->size == 0;
}

u64 queueGetSize(const Queue *q)
{
	return q->size;
}

void queuePrint(const Queue *q, void printData(const void *))
{
	circularListPrint(q->cl, printData);
}

void queueClear(Queue *q, void freeData(void *))
{
	circularListFree(q->cl, freeData);
	q->size = 0;
	q->cl = NULL;
}

void queueFree(Queue *q, void freeData(void *))
{
	circularListFree(q->cl, freeData);
	free(q);
}

circularList *queueToCircularList(Queue *q)
{
	return q->cl;
}
