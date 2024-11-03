#ifndef STACK_H
#define STACK_H
#include "shared.h"

typedef struct {
	void **data;
	size_t len;
	size_t capacity;
} stack;

#define MIN_STACK_CAPACITY (16)

stack newStack();
stack newStackWithCapacity(const size_t capacity);
void stackPush(stack *s, void *data);
void *stackPop(stack *s);
void *stackTop(const stack *s);
size_t stackGetSize(const stack *s);
bool stackIsEmpty(const stack *s);
void stackShrinkToFit(stack *s);
void stackClear(stack *s);
void stackFree(stack *s, void (*freeData)(void *));

#endif
