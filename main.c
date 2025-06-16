#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "libzatar.h"

Z_AVL_DECLARE(Avl_Node, void *, void *, avl);
Z_AVL_IMPLEMENT(Avl_Node, void *, void *, avl);

int main(void)
{
    // Z_String fileContent = {0};

    // if (!z_read_whole_file("main.c", &fileContent)) {
    //     return 1;
    // }

    // Z_String_View delim = Z_CSTR_TO_SV("\n");
    // Z_String_View line = z_str_tok_start(Z_STR_TO_SV(fileContent), delim);

    // while (line.len > 0) {
    //     printf("-- ");
    //     z_str_println(line);
    //     line = z_str_tok_next(Z_STR_TO_SV(fileContent), line, delim);
    // }

    // z_str_free(&fileContent);

    Z_String s = z_str_new_format("hey    ");

    z_str_trim(&s);
    printf("'%s'\n", z_str_to_cstr(&s));




}
