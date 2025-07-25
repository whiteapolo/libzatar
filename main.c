#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "libzatar.h"

int *d(int a) {
  int *p = malloc(sizeof(int));
  *p = a;
  return p;
}

int main() {
  Z_Map map = {.compare_keys = (Z_Compare_Fn)strcmp};
  z_map_put(&map, d(10), d(5), free, free);

  int *value;

  if (z_map_find(&map, d(10), &value)) {
    printf("%d\n", *value);
  }
}
