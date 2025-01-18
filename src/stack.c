#include <string.h>
#include "stack.h"

Stack *newStack()
{
	return newStackWithCapacity(MIN_STACK_CAPACITY);
}

Stack *newStackWithCapacity(const u64 capacity)
{
	return newVecWithCapacity(capacity);
}

void stackPush(Stack *s, void *data)
{
	vecAdd(s, data);
}

void *stackPop(Stack *s)
{
	return vecRemoveLast(s);
}

const void *stackTop(const Stack *s)
{
	return vecAt(s, vecSize(s) - 1);
}

u64 stackSize(const Stack *s)
{
	return vecSize(s);
}

bool stackIsEmpty(const Stack *s)
{
	return vecIsEmpty(s);
}

void stackShrinkToFit(Stack *s)
{
	vecShrinkToFit(s);
}

void stackClear(Stack *s, void freeData(void *))
{
	vecClear(s, freeData);
}

void stackFree(Stack *s, void freeData(void *))
{
	vecFree(s, freeData);
}
