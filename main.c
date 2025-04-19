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

	int x = 3;
	Stack *s = newStack();
	stackPush(s, &x);
	stackPush(s, &x);
	stackPush(s, &x);
	stackPush(s, &x);

	stackPrint(s, printInt);



	return 0;
}
