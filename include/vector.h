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
vector vectorClone(const vector *v, void *(*dupData)(const void *));
vector vectorCloneRange(const vector *v, ssize_t start, ssize_t end, const ssize_t step, void *(*dupData)(const void *));

const void *vectorAt(const vector *v, const size_t i);
void vectorAdd(vector *v, void *data);
void *vectorRemoveLast(vector *v);
void vectorInsertAt(vector *v, size_t i, void *data);
void *vectorRemoveAt(vector *v, size_t i);
void vectorRemoveRange(vector *v, ssize_t start, ssize_t end, const ssize_t step, void (*freeData)(void *));

void vectorAddVector(vector *dest, const vector *src, void *(*dupData)(const void *));
void vectorAddVectorAt(vector *dest, const size_t i, const vector *src, void *(*dupData)(const void *));
void *vectorReplace(vector *v, const size_t i, void *data);

size_t vectorSize(const vector *v);
bool vectorIsEmpty(const vector *v);

void vectorShrinkToFit(vector *v);
void vectorClear(vector *v, void (*freeData)(void *));
void vectorFree(vector *v, void (*freeData)(void *));

void vectorSort(vector *v, int (*cmp)(const void *, const void *));
void vectorReverse(vector *v);
void vectorShuffle(vector *v);

void vectorReduce(vector *v, bool (*shouldRemove)(const void *, size_t i));

void vectorTransform(vector *v, void *(*transform)(void *));
void vectorTransformRange(vector *v, void *(*transform)(void *), ssize_t start, ssize_t end, const size_t step);

void vectorForEach(const vector *v, void (*action)(const void *));
void vectorForEachRange(const vector *v, ssize_t start, ssize_t end, const size_t step, void (*action)(const void *));

void vectorPrint(const vector *v, void (*printData)(const void *));

#endif
