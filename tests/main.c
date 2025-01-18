#include <wh/messure_time.h>
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
#include "sizedPriorityQueueTest.h"
#include "circularListTest.h"
#include "vector.h"

int main()
{
	// vector v = newVec();
	// for (int i = 0; i < 100; i++)
	// 	vecAdd(&v, DUP(5));

	// vector v = newVec();
	// for (int i = 0; i < 100; i++)
	// 	vecAdd(&v, ADDRESS(5));



	RUN_TEST(stackTest);
	RUN_TEST(queueTest);
	RUN_TEST(priorityQueueTest);
	RUN_TEST(sizedPriorityQueueTest);
	RUN_TEST(circularListTest);
	return 0;
}
