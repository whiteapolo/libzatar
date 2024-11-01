#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include <stdbool.h>

const char *testName;
const char *errMessage;

const char *GREEN = "\033[1;32m";
const char *RED   = "\033[1;31m";
const char *RESET = "\033[0m";

#define TEST(name) void name()

#define RUN_TEST(name) \
	do { \
		errMessage = NULL; \
		testName = #name; \
		name(#name); \
		if (errMessage != NULL) {\
			printf("[%sFAILED%s] %s: %s\n", RED, RESET, testName, errMessage); \
		} else { \
			printf("[%sPASSED%s] %s\n", GREEN, RESET, testName); \
		} \
	} while (0)

#define EXPECT(condition, msg) \
	do { \
		if ((condition) == false) { \
			errMessage = msg; \
			return; \
		} \
	} while (0) \

#endif
