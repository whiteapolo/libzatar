#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "test.h"
#include "graph.h"
#include "str.h"
#include "path.h"
#include "cursor.h"
#include "map.h"
#include "list.h"
#include "queue.h"
#include "priorityQueue.h"

#include "stackTest.h"
#include "queueTest.h"
#include "priorityQueueTest.h"

int main()
{
	RUN_TEST(stackTest);
	RUN_TEST(queueTest);
	RUN_TEST(priorityQueueTest);
	return 0;
}
