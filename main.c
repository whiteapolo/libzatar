#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wh/messure_time.h>

#define STACK_IMPL
#include "stack.h"

#define GRAPH_IMPL
#include "graph.h"

#define STRING_IMPL
#include "string.h"

#define PATH_IMPL
#include "path.h"

#define CURSOR_IMPL
#include "cursor.h"

#define MAP_IMPL
#include "map.h"

#define LIST_IMPL
#include "list.h"

#define QUEUE_IMPL
#include "queue.h"

#define PRIORITYQUEUE_IMPL
#include "priorityQueue.h"

int main(int argc, char **argv)
{
	string s = newStr("itay mehadav");
	string slice = newStrSlice(s, -1, 2, 1);
	strPrintln(slice);
}
