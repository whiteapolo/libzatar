#include "stack.h"
#include "test.h"

TEST(newStackTest)
{
	stack s = newStack();
	EXPECT(s.len == 0, "len should be 0");
}

TEST(newStackWithCapacityTest)
{
	stack s = newStackWithCapacity(10);
	EXPECT(s.len == 0, "len should be 0");
	EXPECT(s.capacity == 10, "capacity should be 10");
}

TEST(stackPushTest)
{
	stack s = newStack();
	stackPush(&s, DUP(5));
	EXPECT(s.len == 1, "len should be 1");
}

TEST(stackPopTest)
{
	stack s = newStack();
	stackPush(&s, DUP(1));
	stackPush(&s, DUP(6));
	stackPush(&s, DUP(2));
	EXPECT(*(int*)stackPop(&s) == 2, "poped item should be 2");
	EXPECT(*(int*)stackPop(&s) == 6, "poped item should be 6");
	EXPECT(*(int*)stackPop(&s) == 1, "poped item should be 1");
}

TEST(stackTopTest)
{
	stack s = newStack();
	stackPush(&s, DUP(1));
	stackPush(&s, DUP(6));
	stackPush(&s, DUP(2));
	const int *x = stackTop(&s);
	x = stackTop(&s);
	EXPECT(*x == 2, "poped item should be 2");
}

TEST(stackGetSizeTest)
{
	stack s = newStack();
	EXPECT(stackGetSize(&s) == 0, "stack size should be 0");
	stackPush(&s, DUP(2));
	stackPush(&s, DUP(2));
	stackPush(&s, DUP(2));
	EXPECT(stackGetSize(&s) == 3, "stack size should be 3");
}

TEST(stackIsEmptyTest)
{
	stack s = newStack();
	EXPECT(stackIsEmpty(&s) == true, "stack should be empty");
	stackPush(&s, DUP(2));
	EXPECT(stackIsEmpty(&s) == false, "stack should not be empty");
	stackPush(&s, DUP(2));
	stackPush(&s, DUP(2));
	EXPECT(stackIsEmpty(&s) == false, "stack should not be empty");
	stackPop(&s);
	stackPop(&s);
	stackPop(&s);
	EXPECT(stackIsEmpty(&s) == true, "stack should be empty");
}

TEST(stackShrinkToFitTest)
{
	stack s = newStackWithCapacity(10);
	stackPush(&s, NULL);
	stackPush(&s, NULL);
	stackPush(&s, NULL);
	stackShrinkToFit(&s);
	EXPECT(s.capacity == 3, "capacity should be 0");
}

TEST(stackClearTest)
{
	stack s = newStack();
	stackPush(&s, DUP(5));
	stackPush(&s, DUP(5));
	stackPush(&s, DUP(5));
	stackPush(&s, DUP(5));
	stackClear(&s);
	EXPECT(s.len ==  0, "stack len after clear should be 0");
}

TEST(stackTest)
{
	RUN_TEST(newStackTest);
	RUN_TEST(newStackWithCapacityTest);
	RUN_TEST(stackPushTest);
	RUN_TEST(stackPopTest);
	RUN_TEST(stackTopTest);
	RUN_TEST(stackGetSizeTest);
	RUN_TEST(stackIsEmptyTest);
	RUN_TEST(stackShrinkToFitTest);
	RUN_TEST(stackClearTest);
}
