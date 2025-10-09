#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "libzatar.h"

typedef struct {
  const char *lexeme;
  int line;
} Token;

typedef struct {
  bool ok;
  Token value;
} Optional_Token;

typedef struct {
  bool ok;
  Token value;
} Optional_Token;


void fn() {
  ONCE(printf("Hello world!\n"););
  ONCE(printf("Hello world!\n"););
}

void main() {
  fn();
  fn();
}
