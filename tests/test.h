#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include <stdbool.h>

const char *testName;

const char *GREEN = "\033[1;32m";
const char *RED   = "\033[1;31m";
const char *RESET = "\033[0m";

void setTestName(const char *name)
{
	testName = name;
}

#define TEST(name) \
	void name()

#define RUN_TEST(name) \
	setTestName(#name); \
	name(#name)

void EXPECT(bool condition, const char *msg)
{
	if (condition == true)
		printf("[%sPASSED%s] %s: %s\n", GREEN, RESET, testName, msg);
	else
		printf("[%sFAILED%s] %s: %s\n", RED, RESET, testName, msg);
}

#endif
