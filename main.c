#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIBZATAR_IMPLEMENTATION
#include "libzatar.h"

void test_split(const char *input_str, const char *sep_str,
                const char *expected[], size_t expected_count) {
  Z_String_View s = Z_CSTR(input_str);
  Z_String_View sep = Z_CSTR(sep_str);
  Z_String_View word = z_sv_split_start(s, sep);

  size_t i = 0;
  bool result = true;

  do {
    z_sv_println(word);
    assert(i < expected_count);
    assert(z_sv_compare(word, Z_CSTR(expected[i])) == 0);
    i++;
    result = z_sv_split_next(s, sep, &word);
  } while (result);

  assert(i == expected_count);
  printf("✅ Passed: \"%s\" split by \"%s\"\n", input_str, sep_str);
}

int main(void) {
  // const char *e1[] = {"hello", "good", "bye"};
  // test_split("hello,good,bye", ",", e1, 3);

  // const char *e2[] = {"one", "two", ""};
  // test_split("one,two,", ",", e2, 3);

  // const char *e3[] = {"a", "", "b"};
  // test_split("a,,b", ",", e3, 3);

  // const char *e4[] = {"onlyone"};
  // test_split("onlyone", ",", e4, 1);

  // Long delimiters
  const char *e5[] = {"abc", "def", "ghi"};
  test_split("abc<--->def<--->ghi", "<--->", e5, 3);

  const char *e6[] = {"start", "middle", "end"};
  test_split("start[SEP]middle[SEP]end", "[SEP]", e6, 3);

  // Leading/trailing delimiters with long tokens
  const char *e7[] = {"", "x", ""};
  test_split("<xx>x<xx>", "<xx>", e7, 3);

  // No delimiter present
  const char *e8[] = {"abcdef"};
  test_split("abcdef", "::", e8, 1);

  // Only delimiter
  const char *e9[] = {"", ""};
  test_split("::", "::", e9, 2);

  // Empty string
  const char *e10[] = {""};
  test_split("", ",", e10, 1);

  return 0;
}
