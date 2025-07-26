#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "libzatar.h"

int main() {
  Z_String tmp = z_str_new_format("%s", "/usr/lib/qt5/bin/linguist ");
}
