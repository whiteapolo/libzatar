#include <assert.h>
#include <stdio.h>
#include <string.h>
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

const char *GREEN = "\033[1;32m";
const char *RED   = "\033[1;31m";
const char *RESET = "\033[0m";

void expect(const bool condition, const char *msg)
{
	if (condition == false)
		printf("%s%s%s\n", RED, msg, RESET);
}

int cmpInt(const void *a, const void *b)
{
	return *(const int*)a - *(const int*)b;
}

void testStack()
{
	stack s;
	s = newStackWithCapacity(50);
	expect(s.capacity == 50, "stack capacity at start");
	stackShrinkToFit(&s);
	expect(s.capacity == 0, "capacity should be 0 after shrinking");

	s = newStack();
	expect(stackGetSize(&s) == 0, "stack size is not 0 at start");
	expect(stackIsEmpty(&s) == true, "stack size is not empty at start");
	int values[] = { 1, 2, 3, 4, 5 };
	int popedOrder[] = { 5, 4, 3, 2, 1 };
	for (int i = 0; i < ARRAY_SIZE(values); i++)
		stackPush(&s, &values[i]);

	const int *x = stackTop(&s);
	expect(*x == popedOrder[0], "stack top order");

	for (int i = 0; i < ARRAY_SIZE(popedOrder); i++) {
		const int *x = stackPop(&s);
		expect(*x == popedOrder[i], "stack poped order");
	}

	expect(stackIsEmpty(&s) == true, "stack should be empty!");
}

void testPriorityQueue()
{
	priorityQueue q = newPriorityQueue(cmpInt);
	expect(priorityQueueGetSize(&q) == 0, "Priority queue size is not 0 at start");
	expect(priorityQueueIsEmpty(&q) == true, "Priority queue should be empty");

	int values[] = { 4, 1, 3, 5, 2 };
	int expectedOrder[] = { 1, 2, 3, 4, 5 };

	for (int i = 0; i < ARRAY_SIZE(values); i++)
		priorityQueuePush(&q, &values[i]);

	expect(priorityQueueIsEmpty(&q) == false, "Priority queue should be full");
	expect(priorityQueueGetSize(&q) == 5, "Priority queue size is incorrect after pushes");

	const int *top = priorityQueuePeek(&q);
	expect(*top == expectedOrder[0], "Priority queue top element is not correct");

	for (int i = 0; i < ARRAY_SIZE(expectedOrder); i++) {
		const int *x = priorityQueuePop(&q);
		expect(*x == expectedOrder[i], "Priority queue popped order is incorrect");
	}

	expect(priorityQueueGetSize(&q) == 0, "Priority queue size");
	expect(priorityQueueIsEmpty(&q), "Priority queue should be empty after all pops");
	priorityQueueFree(&q, NULL);
}

void testStr()
{
	// newStr()
	string s;
	s = newStr("hey");
	expect(strcmp(s.data, "hey") == 0, "newStr");

	// newStrWithCapacity
	s = newStrWithCapacity(50, "hey");
	expect(s.capacity == 50, "newStr");

	// newStrFromExisting
	s = newStrFromExisting(s);
	expect(strcmp(s.data, "hey") == 0, "newStrFromExisting");

	// newStrFromArray
	char *arr[] = {"computer", "mouse", "cube", "fan"};
	s = newStrFromArray(arr, ARRAY_SIZE(arr), "#");
	expect(strcmp(s.data, "computer#mouse#cube#fan") == 0, "newStrFromArray");

	// newStrSlice
	s = newStr("Hello, World!");
	s = newStrSlice(s, -1, 0, -1);
	expect(strcmp(s.data, "!dlroW ,olleH") == 0, "newStrSlice");

	// sliceStr
	s = newStr("Hello, World!");
	strShrinkToFit(&s);
	expect(s.capacity == strlen("Hello, World!") + 1, "strShrinkToFit");
	strSlice t = sliceStr("Hello, World!", strlen("Hello, World!"));
	expect(strncmp(t.data, "Hello, World!", strlen("Hello, World!")) == 0, "sliceStr");
	expect(t.len == strlen("Hello, World!"), "sliceStr");

	// sliceStrRange
	s = newStr("Hello, World!");
	t = sliceStrRange(s, 0, 5);
	expect(strncmp(t.data, "Hello,", 6) == 0, "sliceStrRange");
	expect(t.len == 6, "sliceStrRange");

	// sliceStrC
	s = newStr("Hello, World!");
	t = sliceStrC("Hello, World!");
	expect(strncmp(t.data, "Hello, World!", strlen("Hello, World!")) == 0, "sliceStrC");
	expect(t.len == strlen("Hello, World!"), "sliceStrC");







}

int main()
{
	testStack();
	testPriorityQueue();
	testStr();
	return 0;
}
