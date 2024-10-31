#include "list.h"

list *newList()
{
	return NULL;
}

void listPush(list **m, void *data)
{
	list *n = malloc(sizeof(list));
	n->next = *m;
	n->data = data;
	*m = n;
}

void *listPop(list **m)
{
	void *data = (*m)->data;
	list *tmp = *m;
	*m = (*m)->next;
	free(tmp);
	return data;
}

void listInsertAfter(list *l, void *data)
{
	list *n = malloc(sizeof(list));
	n->next = l->next;
	n->data = data;
	l->next = n;
}

void *listRemoveAfter(list *l)
{
	list *tmp = l->next;
	void *data = tmp->data;
	l->next = l->next->next;
	free(tmp);
	return data;
}

void listFree(list **m, void (*freeData)(void *))
{
	while (*m != NULL) {
		void *tmp = listPop(m);
		if (freeData)
			freeData(tmp);
	}
}

void listInsertEnd(list *l, void *data)
{
	listInsertAfter((list *)listGetLast(l), data);
}

void listPrint(const list *l, void (*printData)(void*))
{
	while (l) {
		printData(l->data);
		l = l->next;
	}
}

const list *listGetLast(const list *l)
{
	while (l->next)
		l =  l->next;
	return l;
}

void listReverse(list **lst)
{
	list *right = *lst;
	list *left = *lst;

	while (right->next != NULL) {
		void *tmp = listRemoveAfter(right);
		listPush(&left, tmp);
	}

	*lst = left;
}
