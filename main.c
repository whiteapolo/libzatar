#include <stdio.h>
#include <stdbool.h>
#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

void str_debug(z_str s)
{
	printf("s.len = %d\n", s.len);
	// printf("s.capacity = %d\n", s.capacity);
}

int main(void)
{
	z_str s = z_str_new("6969");
	z_str_push(&s, "oooror %s", "hello");
	z_str_push_c(&s, '$');
	z_str_println(s);
	z_str_free(&s);

	return 0;
}
