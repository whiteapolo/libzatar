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

#include <time.h>

int main(int argc, char **argv)
{
	START_CLOCK();
	string s = newStr("");
	for (int i = 0; s.len < 100000000; i++) {
		strPushc(&s, '5');
	}
	END_CLOCK();
	strFree(&s);

// 	FILE *fp = fopen("main.c", "r");
// 	if (fp == NULL)
// 		return 0;
// 	Scanner scanner = newScanner(fp);
// 	const string *line;
// 	while ((line = scannerNextLine(&scanner)) != NULL) {
// 		printf("%s\n", line->data);
// 	}

// 	scannerFree(&scanner);
// 	fclose(fp);
// 	return 0;
}
