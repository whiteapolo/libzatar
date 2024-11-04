#ifndef SHARED_H
#define SHARED_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ADDRESS(element) (&((typeof(element)){element}))
#define DUP(value) memdup(ADDRESS(value), sizeof(value))

typedef enum { Ok = 0, Err = -1, } Result;

int zatarMax(const int a, const int b);
int zatarMin(const int a, const int b);
int zatarMin3(const int a, const int b, const int c);
int zatarMax3(const int a, const int b, const int c);

void swap(void *a, void *b, const size_t size);
size_t getFileSize(FILE *fp);
int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);
void *memdup(const void *mem, const size_t size);

#endif
