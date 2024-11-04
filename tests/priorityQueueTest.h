#include "priorityQueue.h"
#include "test.h"

int priorityQueueTestCmpInt(const void *a, const void *b)
{
	return *(const int*)a - *(const int*)b;
}

TEST(newPriorityQueueTest)
{
	priorityQueue pq = newPriorityQueue(priorityQueueTestCmpInt);
	EXPECT(pq.size == 0, "size should be 0");
}

TEST(newPriorityQueueWithCapacityTest)
{
	priorityQueue pq = newPriorityQueueWithCapacity(priorityQueueTestCmpInt, 20);
	EXPECT(pq.capacity == 20, "size should be 0");
}

TEST(priorityQueuePushTest)
{
	priorityQueue pq = newPriorityQueue(priorityQueueTestCmpInt);
	priorityQueuePush(&pq, DUP(1));
	priorityQueuePush(&pq, DUP(4));
	priorityQueuePush(&pq, DUP(9));
	priorityQueuePush(&pq, DUP(8));
	priorityQueuePush(&pq, DUP(2));
	EXPECT(pq.size == 5, "size should be 1");
	EXPECT(*(int*)priorityQueuePeek(&pq) == 1, "peek should return 1");
}

TEST(priorityQueuePopTest)
{
	priorityQueue pq = newPriorityQueue(priorityQueueTestCmpInt);
	priorityQueuePush(&pq, DUP(1));
	priorityQueuePush(&pq, DUP(4));
	priorityQueuePush(&pq, DUP(9));
	priorityQueuePush(&pq, DUP(8));
	priorityQueuePush(&pq, DUP(2));
	EXPECT(*(int*)priorityQueuePop(&pq) == 1, "pop should return 1");
	EXPECT(pq.size == 4, "size should be 4");
	EXPECT(*(int*)priorityQueuePop(&pq) == 2, "pop should return 2");
	EXPECT(pq.size == 3, "size should be 3");
	EXPECT(*(int*)priorityQueuePop(&pq) == 4, "pop should return 4");
	EXPECT(pq.size == 2, "size should be 2");
	EXPECT(*(int*)priorityQueuePop(&pq) == 8, "pop should return 8");
	EXPECT(pq.size == 1, "size should be 1");
	EXPECT(*(int*)priorityQueuePop(&pq) == 9, "pop should return 9");
	EXPECT(pq.size == 0, "size should be 0");
}

TEST(priorityQueuePeekTest)
{
	priorityQueue pq = newPriorityQueue(priorityQueueTestCmpInt);
	priorityQueuePush(&pq, DUP(9));
	priorityQueuePush(&pq, DUP(5));
	priorityQueuePush(&pq, DUP(7));
	priorityQueuePush(&pq, DUP(3));
	priorityQueuePush(&pq, DUP(10));
	EXPECT(*(int*)priorityQueuePeek(&pq) == 3, "peek should return 3");
}

TEST(priorityQueueGetSizeTest)
{
	priorityQueue pq = newPriorityQueue(priorityQueueTestCmpInt);
	for (int i = 0; i < 100; i++)
		priorityQueuePush(&pq, DUP(i));
	EXPECT(*(int*)priorityQueuePeek(&pq) == 0, "peek should be 0");
	EXPECT(priorityQueueGetSize(&pq) == 100, "size should be 100");
	EXPECT(priorityQueueGetSize(&pq) == pq.size, "size should be 100");
}

TEST(priorityQueueIsEmptyTest)
{
	priorityQueue pq = newPriorityQueue(priorityQueueTestCmpInt);
	EXPECT(priorityQueueIsEmpty(&pq) == true, "should be empty at start");
	priorityQueuePush(&pq, DUP(2));
	EXPECT(priorityQueueIsEmpty(&pq) == false, "should not be empty");
	priorityQueuePush(&pq, DUP(5));
	priorityQueuePush(&pq, DUP(9));
	priorityQueuePop(&pq);
	priorityQueuePop(&pq);
	EXPECT(priorityQueueIsEmpty(&pq) == false, "should not be empty");
	priorityQueuePop(&pq);
	EXPECT(priorityQueueIsEmpty(&pq) == true, "should be empty at start");
}

TEST(priorityQueueClearTest)
{
	priorityQueue pq = newPriorityQueue(priorityQueueTestCmpInt);
	priorityQueuePush(&pq, DUP(1));
	priorityQueuePush(&pq, DUP(2));
	priorityQueueClear(&pq, NULL);
	EXPECT(priorityQueueIsEmpty(&pq) == true, "should be empty after clear");
}

TEST(priorityQueueShrinkToFitTest)
{
	priorityQueue pq = newPriorityQueueWithCapacity(priorityQueueTestCmpInt, 20);
	priorityQueuePush(&pq, DUP(4));
	priorityQueuePush(&pq, DUP(3));
	priorityQueuePush(&pq, DUP(2));
	priorityQueuePush(&pq, DUP(1));
	priorityQueueShrinkToFit(&pq);
	EXPECT(pq.capacity == 4, "capacity should be 4");
}

TEST(priorityQueueTest)
{
	RUN_TEST(newPriorityQueueTest);
	RUN_TEST(newPriorityQueueWithCapacityTest);
	RUN_TEST(priorityQueuePushTest);
	RUN_TEST(priorityQueuePopTest);
	RUN_TEST(priorityQueuePeekTest);
	RUN_TEST(priorityQueueGetSizeTest);
	RUN_TEST(priorityQueueIsEmptyTest);
	RUN_TEST(priorityQueueShrinkToFitTest);
	RUN_TEST(priorityQueueClearTest);
}
