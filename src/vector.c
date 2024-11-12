#include "vector.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

vector newVecWithCapacity(const size_t initialCapacity)
{
	vector v;
	v.size = 0;
	v.capacity = initialCapacity;
	v.data = malloc(sizeof(void *) * initialCapacity);
	return v;
}

vector newVec()
{
	return newVecWithCapacity(MIN_VECTOR_CAPACITY);
}

const void *vecAt(const vector *v, const size_t i)
{
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

void vecInsertAt(vector *v, size_t i, void *data)
{
	v->size++;
	if (v->size > v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(void *) * v->capacity);
	}
	memmove(&v->data[i+1], &v->data[i], sizeof(void *) * (v->size - i - 1));
	v->data[i] = data;
}

void *vecRemoveAt(vector *v, size_t i)
{
	void *dataToRemove = v->data[i];
	memmove(&v->data[i], &v->data[i+1], sizeof(void *) * (v->size - i - 1));
	v->size--;
	return dataToRemove;
}

void *vecReplace(vector *v, const size_t i, void *data)
{
	void *tmp = v->data[i];
	v->data[i] = data;
	return tmp;
}

void vecSwap(vector *v, const size_t i, const size_t j)
{
	swap(&v->data[i], &v->data[j], sizeof(void *));
}

void vecRemoveRange(vector *v, ssize_t start, ssize_t end, const ssize_t step, void (*freeData)(void *))
{
	int indexs[vecSize(v)];
	for (int i = 0; i < vecSize(v); i++)
		indexs[i] = i;

	if (start < 0)
		start += vecSize(v);
	if (end < 0)
		end += vecSize(v);
	assert(step != 0);

	ssize_t i = start;
	while (i != end && i >= 0 && i < (ssize_t)vecSize(v)) {
		indexs[i] = -1;
		if (freeData != NULL)
			freeData((void *)vecAt(v, i));
		i += step;
	}

	vector tmp = newVecWithCapacity(vecSize(v));

	for (int i = 0; i < vecSize(v); i++)
		if (indexs[i] != -1)
			vecAdd(&tmp, vecAt(v, i));

}

void vecAddVec(vector *dest, vector *src)
{
	for (size_t i = 0; i < vecSize(src); i++)
		vecAdd(dest, (void *)vecAt(src, i));
	vecFree(src, NULL);
}

void vecAddVecAt(vector *dest, const size_t i, vector *src)
{
	const size_t oldDestSize = dest->size;
	dest->size += src->size;
	if (dest->size > dest->capacity) {
		dest->capacity = dest->size * 2;
		dest->data = realloc(dest->data, sizeof(void *) * dest->capacity);
	}

	memmove(&dest->data[i + src->size], &dest->data[i], sizeof(void *) * (oldDestSize - i));
	memcpy(&dest->data[i], &src->data[0], src->size);
	vecFree(src, NULL);
}

size_t vecSize(const vector *v)
{
	return v->size;
}

bool vecIsEmpty(const vector *v)
{
	return vecSize(v) == 0;
}

vector vecClone(const vector *v, void *(*dupData)(const void *))
{
	vector new = newVecWithCapacity(vecSize(v));
	for (size_t i = 0; i < vecSize(v); i++)
		vecAdd(&new, dupData(vecAt(v, i)));
	return new;
}

vector vecCloneRange(const vector *v, ssize_t start, ssize_t end, const ssize_t step, void *(*dupData)(const void *))
{
	if (start < 0)
		start += vecSize(v);
	if (end < 0)
		end += vecSize(v);
	assert(step != 0);

	vector clone = newVecWithCapacity(vecSize(v));
	ssize_t i = start;
	while (i != end && i >= 0 && i < (ssize_t)vecSize(v)) {
		vecAdd(&clone, dupData(vecAt(v, i)));
		i += step;
	}
	return clone;
}

void vecShrinkToFit(vector *v)
{
	v->capacity = v->size;
	v->data = realloc(v->data, sizeof(void *) * v->capacity);
}

void vecClear(vector *v, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < vecSize(v); i++)
			freeData((void *)vecAt(v, i));
	v->size = 0;
}

void vecFree(vector *v, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < vecSize(v); i++)
			freeData((void *)vecAt(v, i));
	free(v->data);
}

void vecSort(vector *v, int (*cmp)(const void *, const void *))
{
	qsort(v->data, v->size, sizeof(void *), cmp);
}

void vecReverse(vector *v)
{
	size_t start = 0;
	size_t end = vecSize(v) - 1;

	while (start < end)
		vecSwap(v, start++, end--);
}

void vecShuffle(vector *v)
{
	srand(time(NULL));
	for (size_t i = 0; i < vecSize(v); i++) {
		const size_t j = rand() % vecSize(v);
		vecSwap(v, i, j);
	}
}

void vecReduce(vector *v, bool (*shouldRemove)(const void *, size_t i), void (*freeData)(void *))
{
	vector tmp = newVecWithCapacity(v->capacity);
	for (size_t i = 0; i < vecSize(v); i++) {
		if (!shouldRemove(vecAt(v, i), i))
			vecAdd(&tmp, (void *)vecAt(v, i));
		else if (freeData != NULL)
			freeData((void *)vecAt(v, i));
	}
	vecFree(v, NULL);
	*v = tmp;
}

void vecTransform(vector *v, void *(*transform)(const void *))
{
	for (size_t i = 0; i < vecSize(v); i++)
		vecReplace(v, i, transform(vecAt(v, i)));
}

void vecTransformRange(vector *v, void *(*transform)(const void *), ssize_t start, ssize_t end, const size_t step)
{
	if (start < 0)
		start += vecSize(v);
	if (end < 0)
		end += vecSize(v);
	assert(step != 0);

	ssize_t i = start;
	while (i != end && i >= 0 && i < (ssize_t)vecSize(v)) {
		vecReplace(v, i, transform(vecAt(v, i)));
		i += step;
	}
}

void vecForEach(const vector *v, void (*action)(const void *))
{
	for (size_t i = 0; i < vecSize(v); i++)
		action(vecAt(v, i));
}

void vecForEachRange(const vector *v, ssize_t start, ssize_t end, const size_t step, void (*action)(const void *))
{
	if (start < 0)
		start += vecSize(v);
	if (end < 0)
		end += vecSize(v);
	assert(step != 0);

	ssize_t i = start;
	while (i != end && i >= 0 && i < (ssize_t)vecSize(v)) {
		action(vecAt(v, i));
		i += step;
	}
}

void vecPrint(const vector *v, void (*printData)(const void *))
{
	printf("[ ");
	vecForEach(v, printData);
	printf(" ]");
}
