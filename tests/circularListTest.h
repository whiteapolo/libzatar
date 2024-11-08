#include "test.h"
#include "circularList.h"

TEST(newCircularListTest)
{
	circularList *cl = newCircularList();
	EXPECT(cl == NULL, "circularList should be NULL at start");
}

TEST(circularListPushTest)
{
	circularList *cl = newCircularList();
	int v1 = 42, v2 = 30, v3 = 15;

	circularListPush(&cl, &v1);
	EXPECT(cl != NULL, "circularList should not be NULL after push");
	EXPECT(cl->data == &v1, "First pushed data should match the pushed value");
	EXPECT(cl->next == cl, "Only element should point to itself");

	circularListPush(&cl, &v2);
	EXPECT(cl->data == &v1, "Head data should still match first pushed value");
	EXPECT(cl->next->data == &v2, "Second element's data should match second pushed value");
	EXPECT(cl->next->next == cl, "List should loop back to head after two elements");

	circularListPush(&cl, &v3);
	EXPECT(cl->next->data == &v3, "Third element's data should match third pushed value");
	EXPECT(cl->next->next->next == cl, "List should loop back to head after three elements");

	circularListFree(cl, NULL);
}

TEST(circularListPopTest)
{
	circularList *cl = newCircularList();
	int v1 = 42, v2 = 99, v3 = 15;
	circularListPush(&cl, &v1);
	circularListPush(&cl, &v2);
	circularListPush(&cl, &v3);

	int *poppedValue = circularListPop(&cl);
	EXPECT(poppedValue == &v3, "Popped value should be the last pushed element");
	EXPECT(cl->next->data == &v2, "Remaining head should match the second pushed value");

	poppedValue = circularListPop(&cl);
	EXPECT(poppedValue == &v2, "Second pop should return second pushed value");
	EXPECT(cl->data == &v1, "Only remaining element should match the first pushed value");
	EXPECT(cl->next == cl, "Single remaining element should point to itself");

	circularListFree(cl, NULL);
}

TEST(circularListInsertAfterTest)
{
	circularList *cl = newCircularList();
	int v1 = 10, v2 = 20, v3 = 30;

	circularListPush(&cl, &v1);
	circularListInsertAfter(cl, &v2);
	EXPECT(cl->next->data == &v2, "First insertion after head should match inserted value");
	EXPECT(cl->next->next == cl, "Inserted node should point back to head");

	circularListInsertAfter(cl->next, &v3);
	EXPECT(cl->next->next->data == &v3, "Second insertion after first node should match inserted value");
	EXPECT(cl->next->next->next == cl, "Newly inserted node should loop back to head");

	circularListFree(cl, NULL);
}

TEST(circularListRemoveAfterTest)
{
	circularList *cl = newCircularList();
	int v1 = 10, v2 = 20, v3 = 30;

	circularListPush(&cl, &v1);
	circularListInsertAfter(cl, &v2);
	circularListInsertAfter(cl->next, &v3);

	void *removedValue = circularListRemoveAfter(cl);
	EXPECT(removedValue == &v2, "Removed value should match the first inserted value");
	EXPECT(cl->next->data == &v3, "Remaining list should only contain head and second inserted value");

	removedValue = circularListRemoveAfter(cl);
	EXPECT(removedValue == &v3, "Removed value should match the second inserted value");
	EXPECT(cl->next == cl, "Only head should remain, pointing to itself");

	circularListFree(cl, NULL);
}

TEST(circularListComprehensiveTest)
{
	circularList *cl = newCircularList();
	int v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5;

	circularListPush(&cl, &v1);
	circularListPush(&cl, &v2);
	circularListPush(&cl, &v3);

	void *poppedValue = circularListPop(&cl);
	EXPECT(poppedValue == &v3, "First popped value should be last pushed (3)");
	EXPECT(cl->next->next == cl, "After one pop, list should have two elements");

	circularListInsertAfter(cl, &v4);
	circularListInsertAfter(cl->next, &v5);
	EXPECT(cl->next->data == &v4, "After insertion, first element should be 4");
	EXPECT(cl->next->next->data == &v5, "After insertion, second element should be 5");
	EXPECT(cl->next->next->next->next == cl, "List should loop back to head");

	circularListFree(cl, NULL);
}

TEST(circularListFreeTest)
{
	circularList *cl = newCircularList();
	int value = 42;
	circularListPush(&cl, &value);
	circularListFree(cl, NULL);
	EXPECT(cl != NULL, "Circular list should be freed without error");
}

TEST(circularListTest)
{
	RUN_TEST(newCircularListTest);
	RUN_TEST(circularListPushTest);
	RUN_TEST(circularListPopTest);
	RUN_TEST(circularListInsertAfterTest);
	RUN_TEST(circularListRemoveAfterTest);
	RUN_TEST(circularListComprehensiveTest);
	RUN_TEST(circularListFreeTest);
}
