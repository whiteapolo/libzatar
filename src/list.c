#include "list.h"

List *newList()
{
	return NULL;
}

void listPush(List **m, void *data)
{
	List *n = malloc(sizeof(List));
	n->next = *m;
	n->data = data;
	*m = n;
}

void *listPop(List **m)
{
	void *data = (*m)->data;
	List *tmp = *m;
	*m = (*m)->next;
	free(tmp);
	return data;
}

void listInsertAfter(List *l, void *data)
{
	List *n = malloc(sizeof(List));
	n->next = l->next;
	n->data = data;
	l->next = n;
}

void *listRemoveAfter(List *l)
{
	List *tmp = l->next;
	void *data = tmp->data;
	l->next = l->next->next;
	free(tmp);
	return data;
}

void listFree(List *m, void (*freeData)(void *))
{
	while (m != NULL) {
		void *tmp = listPop(&m);
		if (freeData)
			freeData(tmp);
	}
}

void listInsertEnd(List *l, void *data)
{
	listInsertAfter((List *)listGetLast(l), data);
}

void listPrint(const List *l, void (*printData)(const void *))
{
	while (l) {
		printData(l->data);
		l = l->next;
	}
}

const List *listGetLast(const List *l)
{
	while (l->next)
		l =  l->next;
	return l;
}

void listReverse(List **lst)
{
	List *right = *lst;
	List *left = *lst;

	while (right->next != NULL) {
		void *tmp = listRemoveAfter(right);
		listPush(&left, tmp);
	}

	*lst = left;
}
