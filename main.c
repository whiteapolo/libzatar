#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

Z_VECTOR_DECLARE(VecI, int, veci);
Z_VECTOR_IMPLEMENT(VecI, int, veci);

void print_int(int a)
{
	printf("%d", a);
}

int main(void)
{
	srand(time(NULL));

	VecI v;
	veci_init(&v);

	for (int i = 0; i < 10; i++) {
		veci_add(&v, rand() % 100);
	}

	veci_print(&v, print_int);
	veci_free(&v, NULL);
}
