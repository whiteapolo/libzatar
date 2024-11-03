#include "queue.h"
#include "test.h"

TEST(newQueueTest)
{
	queue q = newQueue();
	EXPECT(queueIsEmpty(&q) == true, "queue should be empty at start");
}

TEST(enqueueTest)
{
	queue q = newQueue();
	enqueue(&q, NULL);
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after enqueue");
}

TEST(dequeueTest)
{
	queue q = newQueue();
	enqueue(&q, DUP(1));
	enqueue(&q, DUP(2));
	enqueue(&q, DUP(3));
	enqueue(&q, DUP(4));
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after enqueue");
	EXPECT(*(int*)dequeue(&q) = 1, "dequeue should be 1");
	EXPECT(*(int*)dequeue(&q) = 2, "dequeue should be 2");
	EXPECT(*(int*)dequeue(&q) = 3, "dequeue should be 3");
	EXPECT(*(int*)dequeue(&q) = 4, "dequeue should be 4");
	EXPECT(queueIsEmpty(&q) == true, "queue should be empty after enqueue");
}

TEST(queueIsEmptyTest)
{
	queue q = newQueue();
	enqueue(&q, DUP(1));
	enqueue(&q, DUP(2));
	enqueue(&q, DUP(3));
	enqueue(&q, DUP(4));
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after enqueue");
	dequeue(&q);
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after enqueue");
	dequeue(&q);
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after enqueue");
	dequeue(&q);
	EXPECT(queueIsEmpty(&q) == false, "queue should not be empty after enqueue");
	dequeue(&q);
	EXPECT(queueIsEmpty(&q) == true, "queue should be empty after enqueue");
}

TEST(queueTest)
{
	RUN_TEST(newQueueTest);
	RUN_TEST(enqueueTest);
	RUN_TEST(dequeueTest);
	RUN_TEST(queueIsEmptyTest);
}
