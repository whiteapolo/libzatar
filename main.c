#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

int main(void)
{
    Z_Str s = z_str_new("");
    z_str_push_c(&s, 'a');

    z_str_println(s);
}
