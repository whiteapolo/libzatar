#ifndef VECTOR_H
#define VECTOR_H
#include "shared.h"

#define MIN_VECTOR_CAPACITY 16

typedef struct {
	void **data;
	size_t size;
	size_t capacity;
} vector;

vector newVec();
vector newVecWithCapacity(const size_t initialCapacity);
vector vecClone(const vector *v, void *(*dupData)(const void *));
vector vecCloneRange(const vector *v, ssize_t start, ssize_t end, const ssize_t step, void *(*dupData)(const void *));

const void *vecAt(const vector *v, const size_t i);
void vecAdd(vector *v, void *data);
void *vecRemoveLast(vector *v);
void vecInsertAt(vector *v, size_t i, void *data);
void *vecRemoveAt(vector *v, size_t i);
void vecRemoveRange(vector *v, ssize_t start, ssize_t end, const ssize_t step, void (*freeData)(void *));

void vecAddVec(vector *dest, vector *src);
void vecAddVecAt(vector *dest, const size_t i, vector *src);
void *vecReplace(vector *v, const size_t i, void *data);
void vecSwap(vector *v, const size_t i, const size_t j);

size_t vecSize(const vector *v);
bool vecIsEmpty(const vector *v);

void vecShrinkToFit(vector *v);
void vecClear(vector *v, void (*freeData)(void *));
void vecFree(vector *v, void (*freeData)(void *));

void vecSort(vector *v, int (*cmp)(const void *, const void *));
void vecReverse(vector *v);
void vecShuffle(vector *v);

void vecReduce(vector *v, bool (*shouldRemove)(const void *, size_t i), void (*freeData)(void *));

void vecTransform(vector *v, void *(*transform)(const void *));
void vecTransformRange(vector *v, void *(*transform)(const void *), ssize_t start, ssize_t end, const size_t step);

void vecForEach(const vector *v, void (*action)(const void *));
void vecForEachRange(const vector *v, ssize_t start, ssize_t end, const size_t step, void (*action)(const void *));

void vecPrint(const vector *v, void (*printData)(const void *));

#endif
