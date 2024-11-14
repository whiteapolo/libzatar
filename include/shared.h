#ifndef SHARED_H
#define SHARED_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define ADDRESS(element) (&((typeof(element)){element}))
#define DUP(value) memdup(ADDRESS(value), sizeof(value))
#define IN_RANGE(min, x, max) ((min) <= (x) && (x) <= (max))

typedef enum { Ok = 0, Err = -1, } Result;

int zatarMax(const int a, const int b);
int zatarMin(const int a, const int b);
int zatarMin3(const int a, const int b, const int c);
int zatarMax3(const int a, const int b, const int c);

int zatarRand(const unsigned int seed);
void swap(void *a, void *b, const size_t size);
size_t getFileSize(FILE *fp);
int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);
void *memdup(const void *mem, const size_t size);

#endif
