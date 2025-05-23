#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

Z_MAP_DECLARE(Map, char *, char *, map)
Z_MAP_IMPLEMENT(Map, char *, char *, map)

int main(void)
{
    Map m;
    map_init(&m, (int (*)(char *, char *))strcmp);

    map_put(&m, strdup("Hello"), strdup("bye"), (void (*)(char *))free);

    char *x;
    if (map_find(&m, "Hello", &x)) {
        printf("%s\n", x);
    }

}
