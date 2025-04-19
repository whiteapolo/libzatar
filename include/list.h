#ifndef LLL_H
#define LLL_H
#include "shared.h"

typedef struct List{
	struct List *next;
	void *data;
} List;

List *newList();
void listPush(List **m, void *data);
void *listPop(List **m);
void listInsertAfter(List *l, void *data);
void *listRemoveAfter(List *l);
void listFree(List *m, void (*freeData)(void *));
void listInsertEnd(List *l, void *data);
void listPrint(const List *l, void (*printData)(const void*));
const List *listGetLast(const List *l);
void listReverse(List **lst);

#endif
