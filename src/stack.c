#include <string.h>
#include "stack.h"

stack newStack()
{
	return newStackWithCapacity(MIN_STACK_CAPACITY);
}

stack newStackWithCapacity(const size_t capacity)
{
	stack s;
	s.capacity = capacity;
	s.data = malloc(sizeof(void*) * s.capacity);
	s.len = 0;
	return s;
}

void stackPush(stack *s, void *data)
{
	if (s->len + 1 > s->capacity) {
		s->capacity *= 2;
		s->data = realloc(s->data, sizeof(void*) * s->capacity);
	}
	s->data[s->len++] = data;
}

void *stackPop(stack *s)
{
	return s->data[--s->len];
}

void *stackTop(const stack *s)
{
	return s->data[s->len - 1];
}

size_t stackGetSize(const stack *s)
{
	return s->len;
}

bool stackIsEmpty(const stack *s)
{
	return s->len == 0;
}

void stackShrinkToFit(stack *s)
{
	s->capacity = s->len;
	s->data = realloc(s->data, sizeof(void*) * s->capacity);
}

void stackClear(stack *s)
{
	s->len = 0;
}

void stackFree(stack *s, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < s->len; i++)
			freeData(s->data[i]);
	free(s->data);
}
