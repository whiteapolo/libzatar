#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include <stdlib.h>

#define ADDRESS(element) (&((typeof(element)){element}))
#define DUP(value) memdup(ADDRESS(value), sizeof(value))

typedef struct {
	void **data;
	size_t len;
	size_t capacity;
} stack;

const size_t MIN_STACK_CAPACITY = 16;

stack newStack();
stack newStackWithCapacity(const size_t capacity);
void stackPush(stack *s, void *data);
void *stackPop(stack *s);
void *stackTop(const stack *s);
size_t stackGetSize(const stack *s);
bool stackIsEmpty(const stack *s);
void stackShrinkToFit(stack *s);
void stackFree(stack *s, void (*freeData)(void *));

#ifdef STACK_IMPL
#include <string.h>

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

void stackFree(stack *s, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < s->len; i++)
			freeData(s->data[i]);
	free(s->data);
}

#ifndef MEMDUP
#define MEMDUP
void *memdup(const void *mem, const size_t size)
{
	void *newMem = malloc(size);
	memcpy(newMem, mem, size);
	return newMem;
}
#endif
#endif
#endif
