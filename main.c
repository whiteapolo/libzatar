#include <stdio.h>
#include <stdbool.h>
#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

int main(void)
{
	FILE *fp = fopen("main.c", "r");

	if (fp == NULL) {
		return 1;
	}

	z_str f = z_read_whole_file(fp);

	z_str_slice line = z_str_tok_init(f, "\n");

	while (z_str_tok_next(f, &line, "\n") != Err) {
		printf("--- ");
		z_str_print(line);
		printf("--- \n");
	}

	z_str_free(&f);

	fclose(fp);

	return 0;
}
