#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "libzatar.h"

static int cmpstringp(const void *p1, const void *p2) {
  /* The actual arguments to this function are "pointers to
     pointers to char", but strcmp(3) arguments are "pointers
     to char", hence the following cast plus dereference */

  return strcmp(*(const char **)p1, *(const char **)p2);
}

int main() {
  Z_File_Paths files = {0};
  Z_Arena arena = {0};
  z_read_whole_dir("../", &files, &arena);

  qsort(files.ptr, files.len, sizeof(char *), cmpstringp);

  z_da_foreach(file, &files) {
    if (**file != '.')
      printf("%s\n", *file);
  }

  z_arena_free_all(&arena);
  z_da_free(&files);
}
