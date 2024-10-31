#ifndef LLL_H
#define LLL_H
#include "shared.h"

typedef struct list{
	struct list *next;
	void *data;
} list;

list *newList();
void listPush(list **m, void *data);
void *listPop(list **m);
void listInsertAfter(list *l, void *data);
void *listRemoveAfter(list *l);
void listFree(list **m, void (*freeData)(void *));
void listInsertEnd(list *l, void *data);
void listPrint(const list *l, void (*printData)(void*));
const list *listGetLast(const list *l);
void listReverse(list **lst);

#endif
