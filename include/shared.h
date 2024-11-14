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

i32 zatarMax(const i32 a, const i32 b);
i32 zatarMin(const i32 a, const i32 b);
i32 zatarMin3(const i32 a, const i32 b, const i32 c);
i32 zatarMax3(const i32 a, const i32 b, const i32 c);

i32 zatarRand(const u32 seed);
void swap(void *a, void *b, const u64 size);
u64 getFileSize(FILE *fp);
i32 getFmtSize(const char *fmt, ...);
i32 getFmtSizeVa(const char *fmt, va_list ap);
void *memdup(const void *mem, const u64 size);

#endif
