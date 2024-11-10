#ifndef VECTOR_H
#define VECTOR_H
#include "shared.h"

#define MIN_VECTOR_CAPACITY 16

typedef struct {
	void **data;
	size_t size;
	size_t capacity;
} vector;

vector newVector();
vector newVectorWithCapacity(const size_t initialCapacity);
vector newVectorShallow(const vector * const v);
vector newVectorDeep(const vector *v, void *(*dupData)(const void *));
vector newVectorShallowRange(const vector *v, ssize_t start, ssize_t end, const ssize_t step);
vector newVectorDeepRange(const vector *v, ssize_t start, ssize_t end, const ssize_t step, void *(*dupData)(const void *));

void *vectorAt(const vector *v, const size_t i);
void vectorPush(vector *v, void *data);
void *vectorPop(vector *v);
void vectorInsert(vector *v, size_t i, void *data);
void *vectorRemove(vector *v, size_t i);
void vectorRemoveRange(vector *v, ssize_t start, ssize_t end, const ssize_t step, void (*freeData)(void *));

void vectorPushShallow(vector *dest, const vector *src);
void vectorPopDeep(vector *dest, const vector *src, void *(*dupData)(const void *));
void vectorInsertShallow(vector *dest, const size_t i, const vector *src);
void vectorInsertDeepCopy(vector *dest, const size_t i, const vector *src, void *(*dupData)(const void *));
void *vectorReplace(vector *v, const size_t i, void *data);

size_t vectorSize(const vector *v);
bool vectorIsEmpty(const vector *v);

void vectorShrinkToFit(vector *v);
void vectorFree(vector *v, void (*freeData)(void *));
void vectorClear(vector *v, void (*freeData)(void *));

void vectorSort(vector *v);
void vectorReverse(vector *v);
void vectorShuffle(vector *v);

void vectorReduce(vector *v, bool (*shouldRemove)(const void *));
void vectorReduceI(vector *v, bool (*shouldRemove)(const void *, size_t i));
void vectorReduceRange(vector *v, bool (*shouldRemove)(const void *), ssize_t start, ssize_t end, const size_t step);
void vectorReduceRangeI(vector *v, bool (*shouldRemove)(const void *, size_t i), ssize_t start, ssize_t end, const size_t step);

void vectorTransform(vector *v, void *(*transform)(void *));
void vectorTransformI(vector *v, void *(*transform)(void *, size_t i));
void vectorTransformRange(vector *v, void *(*transform)(void *), ssize_t start, ssize_t end, const size_t step);
void vectorTransformRangeI(vector *v, void *(*transform)(void *, size_t i), ssize_t start, ssize_t end, const size_t step);

void vectorForEach(const vector *v, void (*action)(const void *));
void vectorForEachI(const vector *v, void (*action)(const void *, size_t i));
void vectorForEachRange(const vector *v, ssize_t start, ssize_t end, const size_t step, void (*action)(const void *));
void vectorForEachRangeI(const vector *v, ssize_t start, ssize_t end, const size_t step, void (*action)(const void *, size_t i));

void vectorPrint(const vector *v, void (*printData)(const void *));
void vectorPrintI(const vector *v, void (*printData)(const void *, size_t i));

#endif
