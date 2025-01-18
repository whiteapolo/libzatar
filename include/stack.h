#ifndef STACK_H
#define STACK_H
#include "shared.h"
#include "vector.h"

typedef Vector Stack;

#define MIN_STACK_CAPACITY 4

Stack *newStack();
Stack *newStackWithCapacity(const u64 capacity);
void stackPush(Stack *s, void *data);
void *stackPop(Stack *s);
const void *stackTop(const Stack *s);
u64 stackSize(const Stack *s);
bool stackIsEmpty(const Stack *s);
void stackShrinkToFit(Stack *s);
void stackClear(Stack *s, void freeData(void *));
void stackFree(Stack *s, void freeData(void *));

#endif
