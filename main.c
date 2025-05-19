#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

Z_VECTOR_DECLARE(Vecd, double, vecd);
Z_VECTOR_IMPLEMENT(Vecd, double, vecd);

int main(void)
{
	Vecd v;
	vecd_init(&v);

	vecd_add(&v, 5.0);

	Z_Str s;

	if (z_read_whole_file(&s, "main.c") == Z_Err) {
		return -1;
	}

	Z_Str_Slice line = z_str_tok_init(s);

	while (z_str_tok_next(s, &line, "\n") == Z_Ok) {
		z_str_println(line);
	}
}
