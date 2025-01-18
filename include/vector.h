#ifndef VECTOR_H
#define VECTOR_H
#include "shared.h"

#define MIN_VECTOR_CAPACITY 4

typedef struct {
	void **data;
	u64 size;
	u64 capacity;
} Vector;

Vector *newVecWithCapacity(const u64 initialCapacity);
Vector *newVec();

const void *vecAt(const Vector *v, const u64 i);
u64 vecSize(const Vector *v);
bool vecIsEmpty(const Vector *v);

void vecAdd(Vector *v, void *data);
void *vecRemoveLast(Vector *v);
void vecInsertAt(Vector *v, u64 i, void *data);
void *vecRemoveAt(Vector *v, u64 i);

void *vecReplace(Vector *v, const u64 i, void *data);
void vecSwap(Vector *v, const u64 i, const u64 j);
void vecRemoveRange(Vector *v, const i64 start, const i64 end, const i64 step, void freeData(void *));
void vecAddVec(Vector *dst, Vector *src);
void vecAddVecAt(Vector *dest, const u64 i, Vector *src);



Vector *vecClone(const Vector *v, void *dupData(const void *));
Vector *vecCloneRange(const Vector *v, const i64 start, const i64 end, const i64 step, void *dupData(const void *));
void vecShrinkToFit(Vector *v);

void vecClear(Vector *v, void freeData(void *));
void vecFree(Vector *v, void freeData(void *));

void vecSort(Vector *v, int cmp(const void *, const void *));
void vecReverse(Vector *v);
void vecShuffle(Vector *v);
void vecReduce(Vector *v, bool shouldRemove(const void *, i64 i), void freeData(void *));
void vecTransform(Vector *v, void *transform(const void *));
void vecTransformRange(Vector *v, const i64 start, const i64 end, const i64 step, void *transform(const void *));
void vecForEach(const Vector *v, void action(const void *));
void vecForEachRange(const Vector *v, const i64 start, const i64 end, const i64 step, void action(const void *));
void vecPrint(const Vector *v, void printData(const void *));

#endif
