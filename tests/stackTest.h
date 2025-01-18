#include "stack.h"
#include "test.h"

TEST(newStackTest)
{
	Stack *s = newStack();
	EXPECT(stackSize(s) == 0, "size should be 0");
}

TEST(newStackWithCapacityTest)
{
	Stack *s = newStackWithCapacity(10);
	EXPECT(stackSize(s) == 0, "size should be 0");
}

TEST(stackPushTest)
{
	Stack *s = newStack();
	stackPush(s, DUP(5));
	EXPECT(stackSize(s) == 1, "size should be 1");
}

TEST(stackPopTest)
{
	Stack *s = newStack();
	stackPush(s, DUP(1));
	stackPush(s, DUP(6));
	stackPush(s, DUP(2));
	EXPECT(*(int*)stackPop(s) == 2, "poped item should be 2");
	EXPECT(*(int*)stackPop(s) == 6, "poped item should be 6");
	EXPECT(*(int*)stackPop(s) == 1, "poped item should be 1");
}

TEST(stackTopTest)
{
	Stack *s = newStack();
	stackPush(s, DUP(1));
	stackPush(s, DUP(6));
	stackPush(s, DUP(2));
	const int *x = stackTop(s);
	x = stackTop(s);
	EXPECT(*x == 2, "poped item should be 2");
}

TEST(stackSizeTest)
{
	Stack *s = newStack();
	EXPECT(stackSize(s) == 0, "Stack *size should be 0");
	stackPush(s, DUP(2));
	stackPush(s, DUP(2));
	stackPush(s, DUP(2));
	EXPECT(stackSize(s) == 3, "Stack *size should be 3");
}

TEST(stackIsEmptyTest)
{
	Stack *s = newStack();
	EXPECT(stackIsEmpty(s) == true, "Stack *should be empty");
	stackPush(s, DUP(2));
	EXPECT(stackIsEmpty(s) == false, "Stack *should not be empty");
	stackPush(s, DUP(2));
	stackPush(s, DUP(2));
	EXPECT(stackIsEmpty(s) == false, "Stack *should not be empty");
	stackPop(s);
	stackPop(s);
	stackPop(s);
	EXPECT(stackIsEmpty(s) == true, "Stack *should be empty");
}

TEST(stackShrinkToFitTest)
{
	Stack *s = newStackWithCapacity(10);
	stackPush(s, NULL);
	stackPush(s, NULL);
	stackPush(s, NULL);
	stackShrinkToFit(s);
}

TEST(stackClearTest)
{
	Stack *s = newStack();
	stackPush(s, DUP(5));
	stackPush(s, DUP(5));
	stackPush(s, DUP(5));
	stackPush(s, DUP(5));
	stackClear(s, free);
	EXPECT(stackSize(s) == 0, "size should be 0");
}

TEST(stackTest)
{
	RUN_TEST(newStackTest);
	RUN_TEST(newStackWithCapacityTest);
	RUN_TEST(stackPushTest);
	RUN_TEST(stackPopTest);
	RUN_TEST(stackTopTest);
	RUN_TEST(stackSizeTest);
	RUN_TEST(stackIsEmptyTest);
	RUN_TEST(stackShrinkToFitTest);
	RUN_TEST(stackClearTest);
}
