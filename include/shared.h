#ifndef SHARED_H
#define SHARED_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum { Ok = 0, Err = -1, } Result;

void swap(void *a, void *b, const size_t size);
size_t getFileSize(FILE *fp);
int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);

#endif
