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
	const int *x = stackPop(&s);
	EXPECT(*x == 2, "poped item should be 2");
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
}
