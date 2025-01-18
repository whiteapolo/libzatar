#include "vector.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

typedef struct {
	i64 start;
	i64 step;
	i64 end;
	i64 i;
} VecIter;

VecIter newVecIter(const Vector *v, const i64 start, const i64 end, const i64 step)
{
	assert(step != 0);

	VecIter vi = {
		.start = start,
		.end = end,
		.step = step,
	};

	// wrap negative values to the end of the array
	if (vi.start < 0)
		vi.start += vecSize(v);
	if (vi.end < 0)
		vi.end += vecSize(v);
	return vi;
}

i64 VecIterBegin(VecIter *vi)
{
	vi->i = vi->start;
	return vi->i;
}

i64 VecIterNext(VecIter *vi)
{
	vi->i += vi->step;
	return vi->i;
}

bool VecIterIsEnd(const VecIter *vi)
{
	if (vi->step > 0 && vi->i > vi->end)
		return true;
	if (vi->step < 0 && vi->i < vi->end)
		return true;
	return false;
}

Vector *newVecWithCapacity(const u64 initialCapacity)
{
	Vector *v = malloc(sizeof(Vector));
	v->size = 0;
	v->capacity = initialCapacity;
	v->data = malloc(sizeof(void *) * initialCapacity);

	return v;
}

Vector *newVec()
{
	return newVecWithCapacity(MIN_VECTOR_CAPACITY);
}

const void *vecAt(const Vector *v, const u64 i)
{
	assert(i < v->size);

	return v->data[i];
}

void vecAdd(Vector *v, void *data)
{
	v->size++;

	if (v->size > v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(void *) * v->capacity);
	}

	v->data[v->size - 1] = data;
}

void *vecRemoveLast(Vector *v)
{
	return v->data[--v->size];
}

void vecInsertAt(Vector *v, u64 i, void *data)
{
	assert(i <= v->size);
	v->size++;

	if (v->size > v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(void *) * v->capacity);
	}

	memmove(&v->data[i+1], &v->data[i], sizeof(void *) * (v->size - i - 1));
	v->data[i] = data;
}

void *vecRemoveAt(Vector *v, u64 i)
{
	assert(i < v->size);
	void *dataToRemove = v->data[i];
	memmove(&v->data[i], &v->data[i+1], sizeof(void *) * (v->size - i - 1));
	v->size--;

	return dataToRemove;
}

void *vecReplace(Vector *v, const u64 i, void *data)
{
	void *tmp = v->data[i];
	v->data[i] = data;
	return tmp;
}

void vecSwap(Vector *v, const u64 i, const u64 j)
{
	swap(&v->data[i], &v->data[j], sizeof(void *));
}

void vecRemoveRange(Vector *v, const i64 start, const i64 end, const i64 step, void freeData(void *))
{
	u8 tmpPtr;
	VecIter vi = newVecIter(v, start, end, step);

	for (i64 i = VecIterBegin(&vi); VecIterIsEnd(&vi); i = VecIterNext(&vi)) {
		if (freeData != NULL)
			freeData(vecReplace(v, i, &tmpPtr));
		else
			vecReplace(v, i, &tmpPtr);
	}

	Vector *tmp = newVecWithCapacity(vecSize(v));
	for (u64 i = 0; i < vecSize(v); i++)
		vecAdd(tmp, (void *)vecAt(v, i));
	vecFree(v, NULL);
	*v = *tmp;
}

void vecAddVec(Vector *dst, Vector *src)
{
	for (u64 i = 0; i < vecSize(src); i++)
		vecAdd(dst, (void *)vecAt(src, i));
	vecFree(src, NULL);
}

void vecAddVecAt(Vector *dest, const u64 i, Vector *src)
{
	const u64 oldDestSize = dest->size;
	dest->size += src->size;

	if (dest->size > dest->capacity) {
		dest->capacity = dest->size * 2;
		dest->data = realloc(dest->data, sizeof(void *) * dest->capacity);
	}

	memmove(&dest->data[i + src->size], &dest->data[i], sizeof(void *) * (oldDestSize - i));
	memcpy(&dest->data[i], &src->data[0], src->size);
	vecFree(src, NULL);
}

u64 vecSize(const Vector *v)
{
	return v->size;
}

bool vecIsEmpty(const Vector *v)
{
	return vecSize(v) == 0;
}

Vector *vecClone(const Vector *v, void *dupData(const void *))
{
	Vector *new = newVecWithCapacity(vecSize(v));
	for (u64 i = 0; i < vecSize(v); i++)
		vecAdd(new, dupData(vecAt(v, i)));
	return new;
}

Vector *vecCloneRange(const Vector *v, const i64 start, const i64 end, const i64 step, void *dupData(const void *))
{
	Vector *clone = newVecWithCapacity(vecSize(v));
	VecIter vi = newVecIter(v, start, end, step);

	for (i64 i = VecIterBegin(&vi); VecIterIsEnd(&vi); i = VecIterNext(&vi))
		vecAdd(clone, dupData(vecAt(v, i)));

	return clone;
}

void vecShrinkToFit(Vector *v)
{
	v->capacity = v->size;
	v->data = realloc(v->data, sizeof(void *) * v->capacity);
}

void vecClear(Vector *v, void freeData(void *))
{
	if (freeData != NULL)
		for (u64 i = 0; i < vecSize(v); i++)
			freeData((void *)vecAt(v, i));
	v->size = 0;
}

void vecFree(Vector *v, void freeData(void *))
{
	if (freeData != NULL)
		for (u64 i = 0; i < vecSize(v); i++)
			freeData((void *)vecAt(v, i));

	free(v->data);
	free(v);
}

void vecSort(Vector *v, int cmp(const void *, const void *))
{
	qsort(v->data, v->size, sizeof(void *), cmp);
}

void vecReverse(Vector *v)
{
	u64 start = 0;
	u64 end = vecSize(v) - 1;

	while (start < end)
		vecSwap(v, start++, end--);
}

void vecShuffle(Vector *v)
{
	u32 seed = time(NULL);
	for (u64 i = 0; i < vecSize(v); i++) {
		seed = zatarRand(seed);
		const i64 j = seed % vecSize(v);
		vecSwap(v, i, j);
	}
}

void vecReduce(Vector *v, bool shouldRemove(const void *, i64 i), void freeData(void *))
{
	Vector *tmp = newVecWithCapacity(v->capacity);
	for (u64 i = 0; i < vecSize(v); i++) {
		void *currData = (void *)vecAt(v, i);
		if (!shouldRemove(currData, i))
			vecAdd(tmp, currData);
		else if (freeData != NULL)
			freeData(currData);
	}
	vecFree(v, NULL);
	*v = *tmp;
}

void vecTransform(Vector *v, void *transform(const void *))
{
	for (u64 i = 0; i < vecSize(v); i++)
		vecReplace(v, i, transform(vecAt(v, i)));
}

void vecTransformRange(Vector *v, const i64 start, const i64 end, const i64 step, void *transform(const void *))
{
	VecIter vi = newVecIter(v, start, end, step);
	for (i64 i = VecIterBegin(&vi); VecIterIsEnd(&vi); i = VecIterNext(&vi))
		vecReplace(v, i, transform(vecAt(v, i)));
}

void vecForEach(const Vector *v, void action(const void *))
{
	for (u64 i = 0; i < vecSize(v); i++)
		action(vecAt(v, i));
}

void vecForEachRange(const Vector *v, const i64 start, const i64 end, const i64 step, void action(const void *))
{
	VecIter vi = newVecIter(v, start, end, step);
	for (i64 i = VecIterBegin(&vi); VecIterIsEnd(&vi); i = VecIterNext(&vi))
		action(vecAt(v, i));
}

void vecPrint(const Vector *v, void printData(const void *))
{
	printf("[ ");
	vecForEach(v, printData);
	printf(" ]");
}
