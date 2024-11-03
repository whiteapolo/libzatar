#ifndef SHARED_H
#define SHARED_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MIN3(a, b, c) MIN(a, MIN(b, c))

#define ADDRESS(element) (&((typeof(element)){element}))
#define DUP(value) memdup(ADDRESS(value), sizeof(value))

typedef enum { Ok = 0, Err = -1, } Result;

void swap(void *a, void *b, const size_t size);
size_t getFileSize(FILE *fp);
int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);
void *memdup(const void *mem, const int size);

#endif
