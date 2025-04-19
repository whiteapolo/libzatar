#include <stdio.h>
#include <stdbool.h>
#define LIBZATAR_IMPL
#include "libzatar.h"

void printInt(const void *x)
{
	printf("%d", *(int*)x);
}

int main(void)
{
	Queue *q = newQueue();

	for(int i = 0; i < 2; i++) {
		queuePush(q, DUP(i));
	}

	printf("%d\n", queueSize(q));

	queuePrint(q, printInt);

	return 0;
}
