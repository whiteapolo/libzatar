#include "shared.h"

int zatarMax(const int a, const int b)
{
	return a > b ? a : b;
}

int zatarMin(const int a, const int b)
{
	return a > b ? b : a;
}

int zatarMin3(const int a, const int b, const int c)
{
	return zatarMin(a, zatarMin(b, c));
}

int zatarMax3(const int a, const int b, const int c)
{
	return zatarMax(a, zatarMax(b, c));
}

void swap(void *a, void *b, const size_t size)
{
	char tmp[size];
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
}

size_t getFileSize(FILE *fp)
{
	const size_t curr = ftell(fp);
	fseek(fp, 0, SEEK_END);
	const size_t size = ftell(fp);
	fseek(fp, curr, SEEK_SET);
	return size;
}

int getFmtSize(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const int size = getFmtSizeVa(fmt, ap);
	va_end(ap);
	return size;
}

int getFmtSizeVa(const char *fmt, va_list ap)
{
	va_list ap1;
	va_copy(ap1, ap);
	const int size = vsnprintf(NULL, 0, fmt, ap1);
	va_end(ap1);
	return size;
}

void *memdup(const void *mem, const size_t size)
{
	void *newMem = malloc(size);
	memcpy(newMem, mem, size);
	return newMem;
}
