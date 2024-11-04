#include "sizedPriorityQueue.h"
#include "test.h"

int sizedPriorityQueueTestCmpInt(const void *a, const void *b)
{
	return *(const int*)a - *(const int*)b;
}

TEST(newSizedPriorityQueueTest)
{
	sizedPriorityQueue pq = newSizedPriorityQueue(20, sizedPriorityQueueTestCmpInt, free);
	EXPECT(sizedPriorityQueueGetSize(&pq) == 0, "size should be 0");
}

TEST(sizedPriorityQueuePushTest)
{
	sizedPriorityQueue pq = newSizedPriorityQueue(20, sizedPriorityQueueTestCmpInt, free);
	sizedPriorityQueuePush(&pq, DUP(1));
	sizedPriorityQueuePush(&pq, DUP(4));
	sizedPriorityQueuePush(&pq, DUP(9));
	sizedPriorityQueuePush(&pq, DUP(8));
	sizedPriorityQueuePush(&pq, DUP(2));
	EXPECT(sizedPriorityQueueGetSize(&pq) == 5, "size should be 5");
	EXPECT(*(int*)sizedPriorityQueuePeek(&pq) == 1, "peek should return 1");
}

TEST(sizedPriorityQueuePopTest)
{
	sizedPriorityQueue pq = newSizedPriorityQueue(20, sizedPriorityQueueTestCmpInt, free);
	sizedPriorityQueuePush(&pq, DUP(1));
	sizedPriorityQueuePush(&pq, DUP(4));
	sizedPriorityQueuePush(&pq, DUP(9));
	sizedPriorityQueuePush(&pq, DUP(8));
	sizedPriorityQueuePush(&pq, DUP(2));
	EXPECT(*(int*)sizedPriorityQueuePop(&pq) == 1, "pop should return 1");
	EXPECT(sizedPriorityQueueGetSize(&pq) == 4, "size should be 4");
	EXPECT(*(int*)sizedPriorityQueuePop(&pq) == 2, "pop should return 2");
	EXPECT(sizedPriorityQueueGetSize(&pq) == 3, "size should be 3");
	EXPECT(*(int*)sizedPriorityQueuePop(&pq) == 4, "pop should return 4");
	EXPECT(sizedPriorityQueueGetSize(&pq) == 2, "size should be 2");
	EXPECT(*(int*)sizedPriorityQueuePop(&pq) == 8, "pop should return 8");
	EXPECT(sizedPriorityQueueGetSize(&pq) == 1, "size should be 1");
	EXPECT(*(int*)sizedPriorityQueuePop(&pq) == 9, "pop should return 9");
	EXPECT(sizedPriorityQueueGetSize(&pq) == 0, "size should be 0");
}

TEST(sizedPriorityQueuePeekTest)
{
	sizedPriorityQueue pq = newSizedPriorityQueue(20, sizedPriorityQueueTestCmpInt, free);
	sizedPriorityQueuePush(&pq, DUP(9));
	sizedPriorityQueuePush(&pq, DUP(5));
	sizedPriorityQueuePush(&pq, DUP(7));
	sizedPriorityQueuePush(&pq, DUP(3));
	sizedPriorityQueuePush(&pq, DUP(10));
	EXPECT(*(int*)sizedPriorityQueuePeek(&pq) == 3, "peek should return 3");
}

TEST(sizedPriorityQueueGetSizeTest)
{
	sizedPriorityQueue pq = newSizedPriorityQueue(20, sizedPriorityQueueTestCmpInt, free);
	for (int i = 0; i < 100; i++)
		sizedPriorityQueuePush(&pq, DUP(i));
	EXPECT(*(int*)sizedPriorityQueuePeek(&pq) == 0, "peek should be 0");
	EXPECT(sizedPriorityQueueGetSize(&pq) == 20, "size should be 20");
}

TEST(sizedPriorityQueueIsEmptyTest)
{
	sizedPriorityQueue pq = newSizedPriorityQueue(20, sizedPriorityQueueTestCmpInt, free);
	EXPECT(sizedPriorityQueueIsEmpty(&pq) == true, "should be empty at start");
	sizedPriorityQueuePush(&pq, DUP(2));
	EXPECT(sizedPriorityQueueIsEmpty(&pq) == false, "should not be empty");
	sizedPriorityQueuePush(&pq, DUP(5));
	sizedPriorityQueuePush(&pq, DUP(9));
	sizedPriorityQueuePop(&pq);
	sizedPriorityQueuePop(&pq);
	EXPECT(sizedPriorityQueueIsEmpty(&pq) == false, "should not be empty");
	sizedPriorityQueuePop(&pq);
	EXPECT(sizedPriorityQueueIsEmpty(&pq) == true, "should be empty at start");
}

TEST(sizedPriorityQueueTest)
{
	RUN_TEST(newSizedPriorityQueueTest);
	RUN_TEST(sizedPriorityQueuePushTest);
	RUN_TEST(sizedPriorityQueuePopTest);
	RUN_TEST(sizedPriorityQueuePeekTest);
	RUN_TEST(sizedPriorityQueueGetSizeTest);
	RUN_TEST(sizedPriorityQueueIsEmptyTest);
}
