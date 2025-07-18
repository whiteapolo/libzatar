#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "libzatar.h"

int main() {

 Z_Arena arena = {0};

    char *s1 = z_arena_malloc(&arena, 10);
    strcpy(s1, "hello");
    printf("s1: %s\n", s1);

    s1 = z_arena_realloc(&arena, s1, 20);
    strcat(s1, " world");
    printf("s1 after realloc: %s\n", s1);

    char *s2 = z_arena_malloc(&arena, 15);
    strcpy(s2, "goodbye");
    printf("s2: %s\n", s2);

    z_arena_free(&arena, s1);
    printf("s1 freed\n");

    z_arena_free_all(&arena);
    printf("arena freed all\n");

    return 0;
}
