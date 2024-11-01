#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "test.h"
#include "stack.h"
#include "graph.h"
#include "str.h"
#include "path.h"
#include "cursor.h"
#include "map.h"
#include "list.h"
#include "queue.h"
#include "priorityQueue.h"

#define ARRAY_SIZE(arr) ((int)(sizeof(arr)/sizeof(arr[0])))

int cmpInt(const void *a, const void *b)
{
	return *(const int*)a - *(const int*)b;
}

TEST(newStackTest) {
	stack s = newStack();
	EXPECT(s.len == 0, "len should be 0");
}

TEST(newStackWithCapacityTest) {
	stack s = newStackWithCapacity(10);
	EXPECT(s.len == 0, "len should be 0");
	EXPECT(s.capacity == 10, "capacity should be 10");
}

TEST(stackPushTest) {
	stack s = newStack();
	stackPush(&s, DUP(5));
	EXPECT(s.len == 1, "len should be 1");
}

TEST(stackPopTest) {
	stack s = newStack();
	stackPush(&s, DUP(1));
	stackPush(&s, DUP(6));
	stackPush(&s, DUP(2));
	const int *x = stackPop(&s);
	EXPECT(*x == 2, "poped item should be 2");
}

TEST(stackTopTest) {
	stack s = newStack();
	stackPush(&s, DUP(1));
	stackPush(&s, DUP(6));
	stackPush(&s, DUP(2));
	const int *x = stackTop(&s);
	x = stackTop(&s);
	EXPECT(*x == 2, "poped item should be 2");
}

TEST(stackGetSizeTest) {
	stack s = newStack();
	EXPECT(stackGetSize(&s) == 0, "stack size should be 0");
	stackPush(&s, DUP(2));
	stackPush(&s, DUP(2));
	stackPush(&s, DUP(2));
	EXPECT(stackGetSize(&s) == 3, "stack size should be 3");
}

int main()
{
	RUN_TEST(newStackTest);
	RUN_TEST(newStackWithCapacityTest);
	RUN_TEST(stackPushTest);
	RUN_TEST(stackPopTest);
	RUN_TEST(stackTopTest);
	RUN_TEST(stackGetSizeTest);
	return 0;
}
