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
} vecIter;

vecIter newVecIter(const vector *v, const i64 start, const i64 end, const i64 step)
{
	assert(step != 0);
	vecIter vi = {
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

i64 vecIterBegin(vecIter *vi)
{
	vi->i = vi->start;
	return vi->i;
}

i64 vecIterNext(vecIter *vi)
{
	vi->i += vi->step;
	return vi->i;
}

bool vecIterIsEnd(const vecIter *vi)
{
	if (vi->step > 0 && vi->i > vi->end)
		return true;
	if (vi->step < 0 && vi->i < vi->end)
		return true;
	return false;
}

vector newVecWithCapacity(const i64 initialCapacity)
{
	vector v = {
		.size = 0,
		.capacity = initialCapacity,
		.data = malloc(sizeof(void *) * initialCapacity),
	};
	return v;
}

vector newVec()
{
	return newVecWithCapacity(MIN_VECTOR_CAPACITY);
}

const void *vecAt(const vector *v, const i64 i)
{
	assert(i < v->size);
	return v->data[i];
}

void vecAdd(vector *v, void *data)
{
	v->size++;
	if (v->size > v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(void *) * v->capacity);
	}
	v->data[v->size - 1] = data;
}

void *vecRemoveLast(vector *v)
{
	return v->data[--v->size];
}

void vecInsertAt(vector *v, i64 i, void *data)
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

void *vecRemoveAt(vector *v, i64 i)
{
	assert(i < v->size);
	void *dataToRemove = v->data[i];
	memmove(&v->data[i], &v->data[i+1], sizeof(void *) * (v->size - i - 1));
	v->size--;
	return dataToRemove;
}

void *vecReplace(vector *v, const i64 i, void *data)
{
	void *tmp = v->data[i];
	v->data[i] = data;
	return tmp;
}

void vecSwap(vector *v, const i64 i, const i64 j)
{
	swap(&v->data[i], &v->data[j], sizeof(void *));
}

void vecRemoveRange(vector *v, const i64 start, const i64 end, const i64 step, void freeData(void *))
{
	u8 tmpPtr;
	vecIter vi = newVecIter(v, start, end, step);
	for (i64 i = vecIterBegin(&vi); vecIterIsEnd(&vi); i = vecIterNext(&vi)) {
		if (freeData != NULL)
			freeData(vecReplace(v, i, &tmpPtr));
		else
			vecReplace(v, i, &tmpPtr);
	}

	vector tmp = newVecWithCapacity(vecSize(v));
	for (i64 i = 0; i < vecSize(v); i++)
		vecAdd(&tmp, (void *)vecAt(v, i));
	vecFree(v, NULL);
	*v = tmp;
}

void vecAddVec(vector *dest, vector *src)
{
	for (i64 i = 0; i < vecSize(src); i++)
		vecAdd(dest, (void *)vecAt(src, i));
	vecFree(src, NULL);
}

void vecAddVecAt(vector *dest, const i64 i, vector *src)
{
	const i64 oldDestSize = dest->size;
	dest->size += src->size;
	if (dest->size > dest->capacity) {
		dest->capacity = dest->size * 2;
		dest->data = realloc(dest->data, sizeof(void *) * dest->capacity);
	}

	memmove(&dest->data[i + src->size], &dest->data[i], sizeof(void *) * (oldDestSize - i));
	memcpy(&dest->data[i], &src->data[0], src->size);
	vecFree(src, NULL);
}

i64 vecSize(const vector *v)
{
	return v->size;
}

bool vecIsEmpty(const vector *v)
{
	return vecSize(v) == 0;
}

vector vecClone(const vector *v, void *dupData(const void *))
{
	vector new = newVecWithCapacity(vecSize(v));
	for (i64 i = 0; i < vecSize(v); i++)
		vecAdd(&new, dupData(vecAt(v, i)));
	return new;
}

vector vecCloneRange(const vector *v, const i64 start, const i64 end, const i64 step, void *dupData(const void *))
{
	vector clone = newVecWithCapacity(vecSize(v));
	vecIter vi = newVecIter(v, start, end, step);
	for (i64 i = vecIterBegin(&vi); vecIterIsEnd(&vi); i = vecIterNext(&vi))
		vecAdd(&clone, dupData(vecAt(v, i)));
	return clone;
}

void vecShrinkToFit(vector *v)
{
	v->capacity = v->size;
	v->data = realloc(v->data, sizeof(void *) * v->capacity);
}

void vecClear(vector *v, void freeData(void *))
{
	if (freeData != NULL)
		for (i64 i = 0; i < vecSize(v); i++)
			freeData((void *)vecAt(v, i));
	v->size = 0;
}

void vecFree(vector *v, void freeData(void *))
{
	if (freeData != NULL)
		for (i64 i = 0; i < vecSize(v); i++)
			freeData((void *)vecAt(v, i));
	free(v->data);
}

void vecSort(vector *v, int cmp(const void *, const void *))
{
	qsort(v->data, v->size, sizeof(void *), cmp);
}

void vecReverse(vector *v)
{
	i64 start = 0;
	i64 end = vecSize(v) - 1;

	while (start < end)
		vecSwap(v, start++, end--);
}

void vecShuffle(vector *v)
{
	u32 seed = time(NULL);
	for (i64 i = 0; i < vecSize(v); i++) {
		seed = zatarRand(seed);
		const i64 j = seed % vecSize(v);
		vecSwap(v, i, j);
	}
}

void vecReduce(vector *v, bool shouldRemove(const void *, i64 i), void freeData(void *))
{
	vector tmp = newVecWithCapacity(v->capacity);
	for (i64 i = 0; i < vecSize(v); i++) {
		void *currData = (void *)vecAt(v, i);
		if (!shouldRemove(currData, i))
			vecAdd(&tmp, currData);
		else if (freeData != NULL)
			freeData(currData);
	}
	vecFree(v, NULL);
	*v = tmp;
}

void vecTransform(vector *v, void *transform(const void *))
{
	for (i64 i = 0; i < vecSize(v); i++)
		vecReplace(v, i, transform(vecAt(v, i)));
}

void vecTransformRange(vector *v, const i64 start, const i64 end, const i64 step, void *transform(const void *))
{
	vecIter vi = newVecIter(v, start, end, step);
	for (i64 i = vecIterBegin(&vi); vecIterIsEnd(&vi); i = vecIterNext(&vi))
		vecReplace(v, i, transform(vecAt(v, i)));
}

void vecForEach(const vector *v, void action(const void *))
{
	for (i64 i = 0; i < vecSize(v); i++)
		action(vecAt(v, i));
}

void vecForEachRange(const vector *v, const i64 start, const i64 end, const i64 step, void action(const void *))
{
	vecIter vi = newVecIter(v, start, end, step);
	for (i64 i = vecIterBegin(&vi); vecIterIsEnd(&vi); i = vecIterNext(&vi))
		action(vecAt(v, i));
}

void vecPrint(const vector *v, void printData(const void *))
{
	printf("[ ");
	vecForEach(v, printData);
	printf(" ]");
}
