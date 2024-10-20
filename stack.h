#ifndef STACK_H
#define STACK_H
#include <stdbool.h>

typedef struct {
	void **data;
	int len;
} stack;

stack newStack();
void stackPush(stack *s, void *data);
void *stackPop(stack *s);
void *stackTop(const stack *s);
bool stackIsEmpty(const stack *s);
void stackFree(stack *s, void (*freeData)(void *));

#ifdef STACK_IMPL
#include <stdlib.h>

stack newStack()
{
	stack s;
	s.data = NULL;
	s.len = 0;
	return s;
}

void stackPush(stack *s, void *data)
{
	s->data = realloc(s->data, sizeof(void*) * ++s->len);
	s->data[s->len - 1] = data;
}

void *stackPop(stack *s)
{
	void *data = s->data[s->len - 1];
	s->data = realloc(s->data, sizeof(void*) * --s->len);
	return data;
}

void *stackTop(const stack *s)
{
	return s->data[s->len - 1];
}

bool stackIsEmpty(const stack *s)
{
	return s->len == 0;
}

void stackFree(stack *s, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (int i = 0; i < s->len; i++)
			freeData(s->data[i]);
	free(s->data);
}

#endif
#endif
