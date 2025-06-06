#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

int main(void)
{
    Z_Result result;
    Z_String f = z_read_whole_file("main.c", &result);

    if (result == Z_Err) {
        return 1;
    }

    Z_String_Tokonizer tok;
    z_str_tok_init(&tok, Z_STR_TO_SV(f), Z_CSTR_TO_SV("\n "));

    Z_String_View line = z_str_tok_next(&tok);

    while (line.len > 0) {
        printf("-- ");
        z_str_println(line);
        line = z_str_tok_next(&tok);
    }

    z_str_free(&f);
}
