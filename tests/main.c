#include <assert.h>
#include <stdio.h>
#include "../stack.h"
#include "../graph.h"
#include "../string.h"
#include "../path.h"
#include "../cursor.h"
#include "../map.h"
#include "../list.h"
#include "../queue.h"
#include "../priorityQueue.h"

const char *GREEN = "\033[1;32m";
const char *RED   = "\033[1;31m";
const char *RESET = "\033[0m";

void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
	fprintf(stderr, "%s", RED);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
	fprintf(stderr, "%s", RESET);
    exit(EXIT_FAILURE);
}

void assertDie(bool condition, const char *msg)
{
	if (condition == false)
		die("%s", msg);
}

int cmpInt(const void *a, const void *b)
{
	return *(const int*)a - *(const int*)b;
}

void testStack()
{
	stack s = newStack();
	assertDie(stackGetSize(&s) == 0, "stack size is not 0 at start");
	int values[] = { 1, 2, 3, 4, 5 };
	int popedOrder[] = { 5, 4, 3, 2, 1 };
	for (int i = 0; i < sizeof(values)/sizeof(values[0]); i++)
		stackPush(&s, &values[i]);

	const int *x = stackTop(&s);
	assertDie(*x == popedOrder[0], "stack top order is not right");

	for (int i = 0; i < sizeof(popedOrder)/sizeof(popedOrder[0]); i++) {
		const int *x = stackPop(&s);
		assertDie(*x == popedOrder[i], "stack poped order is not right");
	}

	assertDie(stackIsEmpty(&s), "stack should be empty!");
	stackFree(&s, NULL);
}

void testPriorityQueue()
{
    priorityQueue q = newPriorityQueue(cmpInt);
    assertDie(priorityQueueGetSize(&q) == 0, "Priority queue size is not 0 at start");

    int values[] = { 4, 1, 3, 5, 2 };
    int expectedOrder[] = { 1, 2, 3, 4, 5 };

    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
        priorityQueuePush(&q, &values[i]);

    assertDie(priorityQueueGetSize(&q) == 5, "Priority queue size is incorrect after pushes");

    const int *top = priorityQueuePeek(&q);
    assertDie(*top == expectedOrder[0], "Priority queue top element is not correct");

    for (int i = 0; i < sizeof(expectedOrder) / sizeof(expectedOrder[0]); i++) {
        const int *x = priorityQueuePop(&q);
        assertDie(*x == expectedOrder[i], "Priority queue popped order is incorrect");
    }

    assertDie(priorityQueueIsEmpty(&q), "Priority queue should be empty after all pops");
    priorityQueueFree(&q, NULL);
}

void testString()
{
	string s1 = newStr("Hello, %s", "world");
    assertDie(strcmp(s1.data, "Hello, world") == 0, "newStr failed");

    string s2 = newStrWithCapacity(50, "Capacity Test");
    assertDie(strcmp(s2.data, "Capacity Test") == 0, "newStrWithCapacity failed");

    strSlice slice1 = sliceStr("Slice this string", 6);
    assertDie(slice1.len == 6, "sliceStr failed");

    strPushc(&s1, '!');
    assertDie(strcmp(s1.data, "Hello, world!") == 0, "strPushc failed");

    char top = strTopc(sliceStrC("Hello"));
    assertDie(top == 'o', "strTopc failed");

    string s3 = newStr("Test");
    strPushs(&s3, ", %s", "push string");
    assertDie(strcmp(s3.data, "Test, push string") == 0, "strPushs failed");

    string s4 = newStr("Trim test   ");
    strTrim(&s4);
    assertDie(strcmp(s4.data, "Trim test") == 0, "strTrim failed");

    assertDie(strCmpC(sliceStrC("Test"), "Test") == 0, "strCmpC failed");

    string s5 = newStr("Upper");
    strToUpper(&s5);
    assertDie(strcmp(s5.data, "UPPER") == 0, "strToUpper failed");

    string s6 = newStr("Lower");
    strToLower(&s6);
    assertDie(strcmp(s6.data, "lower") == 0, "strToLower failed");

    string s7 = newStr("abcdef");
    strReverse(&s7);
    assertDie(strcmp(s7.data, "fedcba") == 0, "strReverse failed");

    string s8 = newStr("Replace this string");
    strReplaceAll(&s8, "string", "text");
    assertDie(strcmp(s8.data, "Replace this text") == 0, "strReplaceAll failed");

    string s9 = newStr("Hello");
    size_t length = strDisplayedLength(sliceStrC(s9.data));
    assertDie(length == 5, "strDisplayedLength failed");

    strFree(&s1);
    strFree(&s2);
    strFree(&s3);
    strFree(&s4);
    strFree(&s5);
    strFree(&s6);
    strFree(&s7);
    strFree(&s8);
    strFree(&s9);
}

int main()
{
	testStack();
	testPriorityQueue();
	testString();
	return 0;
}
