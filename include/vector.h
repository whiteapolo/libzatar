#ifndef VECTOR_H
#define VECTOR_H
#include "shared.h"

#define MIN_VECTOR_CAPACITY 16

typedef struct {
	void **data;
	i64 size;
	i64 capacity;
} vector;

vector newVec();
vector newVecWithCapacity(const i64 initialCapacity);
vector vecClone(const vector *v, void *dupData(const void *));
vector vecCloneRange(const vector *v, const i64 start, const i64 end, const i64 step, void *dupData(const void *));

const void *vecAt(const vector *v, const i64 i);
void vecAdd(vector *v, void *data);
void *vecRemoveLast(vector *v);
void vecInsertAt(vector *v, i64 i, void *data);
void *vecRemoveAt(vector *v, i64 i);
void vecRemoveRange(vector *v, const i64 start, const i64 end, const i64 step, void freeData(void *));

void vecAddVec(vector *dest, vector *src);
void vecAddVecAt(vector *dest, const i64 i, vector *src);
void *vecReplace(vector *v, const i64 i, void *data);
void vecSwap(vector *v, const i64 i, const i64 j);

i64 vecSize(const vector *v);
bool vecIsEmpty(const vector *v);

void vecShrinkToFit(vector *v);
void vecClear(vector *v, void freeData(void *));
void vecFree(vector *v, void freeData(void *));

void vecSort(vector *v, int cmp(const void *, const void *));
void vecReverse(vector *v);
void vecShuffle(vector *v);

void vecReduce(vector *v, bool shouldRemove(const void *, i64 i), void freeData(void *));

void vecTransform(vector *v, void *transform(const void *));
void vecTransformRange(vector *v, const i64 start, const i64 end, const i64 step, void *transform(const void *));

void vecForEach(const vector *v, void action(const void *));
void vecForEachRange(const vector *v, const i64 start, const i64 end, const i64 step, void action(const void *));

void vecPrint(const vector *v, void printData(const void *));

#endif
