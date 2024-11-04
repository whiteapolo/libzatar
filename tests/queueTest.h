#include "queue.h"
#include "test.h"

TEST(newQueueTest)
{
	queue q = newQueue();
	EXPECT(queueIsEmpty(&q) == true, "queue should be empty at start");
}

TEST(queuePushTest)
{
	queue q = newQueue();
	queuePush(&q, NULL);
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after queuePush");
}

TEST(queuePopTest)
{
	queue q = newQueue();
	queuePush(&q, DUP(1));
	queuePush(&q, DUP(2));
	queuePush(&q, DUP(3));
	queuePush(&q, DUP(4));
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after queuePush");
	EXPECT(*(int*)queuePop(&q) = 1, "queuePop should be 1");
	EXPECT(*(int*)queuePop(&q) = 2, "queuePop should be 2");
	EXPECT(*(int*)queuePop(&q) = 3, "queuePop should be 3");
	EXPECT(*(int*)queuePop(&q) = 4, "queuePop should be 4");
	EXPECT(queueIsEmpty(&q) == true, "queue should be empty after queuePush");
}

TEST(queuePeekTest)
{
	queue q = newQueue();
	queuePush(&q, DUP(1));
	queuePush(&q, DUP(2));
	queuePush(&q, DUP(3));
	queuePush(&q, DUP(4));
	EXPECT(*(int*)queuePeek(&q) == 1, "peek should be 1");
	queuePop(&q);
	EXPECT(*(int*)queuePeek(&q) == 2, "peek should be 2");
	queuePop(&q);
	EXPECT(*(int*)queuePeek(&q) == 3, "peek should be 3");
	queuePop(&q);
	EXPECT(*(int*)queuePeek(&q) == 4, "peek should be 4");
	queuePop(&q);
}

TEST(queueIsEmptyTest)
{
	queue q = newQueue();
	queuePush(&q, DUP(1));
	queuePush(&q, DUP(2));
	queuePush(&q, DUP(3));
	queuePush(&q, DUP(4));
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after queuePush");
	queuePop(&q);
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after queuePush");
	queuePop(&q);
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after queuePush");
	queuePop(&q);
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after queuePush");
	queuePop(&q);
	EXPECT(queueIsEmpty(&q) == true, "queue should be empty after queuePush");
}

TEST(queueGetSizeTest)
{
	queue q = newQueue();
	queuePush(&q, DUP(1));
	queuePush(&q, DUP(2));
	queuePush(&q, DUP(3));
	queuePush(&q, DUP(4));
	EXPECT(queueGetSize(&q) == 4, "size should be 4");
	queuePop(&q);
	EXPECT(queueGetSize(&q) == 3, "size should be 3");
	queuePop(&q);
	EXPECT(queueGetSize(&q) == 2, "size should be 2");
	queuePop(&q);
	EXPECT(queueGetSize(&q) == 1, "size should be 1");
	queuePop(&q);
	EXPECT(queueGetSize(&q) == 0, "size should be 0");
}

TEST(queueTest)
{
	RUN_TEST(newQueueTest);
	RUN_TEST(queuePushTest);
	RUN_TEST(queuePopTest);
	RUN_TEST(queuePeekTest);
	RUN_TEST(queueIsEmptyTest);
	RUN_TEST(queueGetSizeTest);
}
