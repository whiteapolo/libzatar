#include <endian.h>
#include <stdlib.h>
#include "circularList.h"

circularList *newCircularList()
{
	return NULL;
}

void circularListPush(circularList **cl, void *data)
{
	circularList *n = malloc(sizeof(circularList));
	n->data = data;
	if (*cl == NULL) {
		n->next = n;
		*cl = n;
	} else if ((*cl)->next == (*cl)) {
		n->next = *cl;
		(*cl)->next = n;
	} else {
		n->next = (*cl)->next;
		(*cl)->next = n;
	}
}

void *circularListPop(circularList **cl)
{
	circularList *toRemove = (*cl)->next;
	void *data = toRemove->data;

	if (*cl == toRemove) {
		*cl = NULL;
	} else {
		(*cl)->next = (*cl)->next->next;
	}

	free(toRemove);

	return data;
}

void circularListInsertAfter(circularList *cl, void *data)
{
	circularList *n = malloc(sizeof(circularList));
	n->data = data;
	n->next = cl->next;
	cl->next = n;
}

void circularListInsertLast(circularList **cl, void *data)
{
	circularList *n = malloc(sizeof(circularList));
	n->data = data;

	if (*cl == NULL) {
		n->next = n;
	} else if ((*cl)->next == (*cl)) {
		n->next = *cl;
		(*cl)->next = n;
	} else {
		n->next = (*cl)->next;
		(*cl)->next = n;
	}

	*cl = n;
}

void *circularListRemoveAfter(circularList *cl)
{
	circularList *toRemove = cl->next;
	cl->next = cl->next->next;
	void *tmp = toRemove->data;
	free(toRemove);

	return tmp;
}

void circularListFree(circularList *cl, void freeData(void *))
{
	if (freeData) {
		while (cl != NULL) {
			freeData(circularListPop(&cl));
		}
	} else {
		while (cl != NULL) {
			circularListPop(&cl);
		}
	}
}

void circularListPrint(circularList *cl, void printData(const void *))
{
	if (cl == NULL) {
		printf("{}\n");
		return;
	}

	printf("{ ");

	for (circularList *curr = cl->next; curr != cl; curr = curr->next) {
		printData(curr->data);
		printf(", ");
	}

	printData(cl->data);

	printf(" }\n");
}
