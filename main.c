#include <stdio.h>
#include <stdbool.h>
#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

VECTOR_DECLARE(z_vector_int, int);
VECTOR_IMPLEMENT(z_vector_int, int);

void print_int(int a)
{
	printf("%d", a);
}

int main(void)
{
	z_vector_int *v = z_vector_int_new();

	z_vector_int_add(v, 5);
	z_vector_int_add(v, 1);
	z_vector_int_add(v, 2);
	z_vector_int_add(v, 6);
	z_vector_int_add(v, 9);

	z_vector_int_print(v, print_int);
	z_vector_int_free(v, NULL);

	return 0;
}
