#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "test.h"
#include "stack.h"
#include "graph.h"
#include "str.h"
#include "path.h"
#include "cursor.h"
#include "map.h"
#include "list.h"
#include "queue.h"
#include "priorityQueue.h"

#include "stackTest.h"

#define ARRAY_SIZE(arr) ((int)(sizeof(arr)/sizeof(arr[0])))

int cmpInt(const void *a, const void *b)
{
	return *(const int*)a - *(const int*)b;
}


int main()
{
	RUN_TEST(stackTest);
	return 0;
}
