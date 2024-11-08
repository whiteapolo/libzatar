#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H
#include "shared.h"

typedef struct circularList {
	struct circularList *next;
	void *data;
} circularList;

circularList *newCircularList();
void circularListPush(circularList **cl, void *data);
void *circularListPop(circularList **cl);
void circularListInsertAfter(circularList *cl, void *data);
void *circularListRemoveAfter(circularList *cl);
void circularListFree(circularList *cl, void (*freeData)(void *));
void circularListPrint(circularList *cl, void (*printData)(const void *));

#endif
