#include "vector.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

vector newVectorWithCapacity(const size_t initialCapacity)
{
	vector v;
	v.size = 0;
	v.capacity = initialCapacity;
	v.data = malloc(sizeof(void *) * initialCapacity);
	return v;
}

vector newVector()
{
	return newVectorWithCapacity(MIN_VECTOR_CAPACITY);
}

const void *vectorAt(const vector *v, const size_t i)
{
	return v->data[i];
}

void vectorAdd(vector *v, void *data)
{
	v->size++;
	if (v->size > v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(void *) * v->capacity);
	}
	v->data[v->size - 1] = data;
}

void *vectorRemoveLast(vector *v)
{
	return v->data[--v->size];
}

void vectorInsertAt(vector *v, size_t i, void *data)
{
	v->size++;
	if (v->size > v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(void *) * v->capacity);
	}
	memmove(&v->data[i+1], &v->data[i], v->size - i - 1);
	v->data[i] = data;
}

void *vectorRemoveAt(vector *v, size_t i)
{
	void *dataToRemove = v->data[i];
	memmove(&v->data[i], &v->data[i+1], v->size - i - 1);
	v->size--;
	return dataToRemove;
}

void *vectorReplace(vector *v, const size_t i, void *data)
{
	void *tmp = v->data[i];
	v->data[i] = data;
	return tmp;
}

void vectorRemoveRange(vector *v, ssize_t start, ssize_t end, const ssize_t step, void (*freeData)(void *));

void vectorAddVector(vector *dest, const vector *src, void *(*dupData)(const void *))
{
	for (size_t i = 0; i < vectorSize(src); i++)
		vectorAdd(dest, dupData(vectorAt(src, i)));
}

void vectorAddVectorAt(vector *dest, const size_t i, const vector *src, void *(*dupData)(const void *));

size_t vectorSize(const vector *v)
{
	return v->size;
}

bool vectorIsEmpty(const vector *v)
{
	return vectorSize(v) == 0;
}

vector vectorClone(const vector *v, void *(*dupData)(const void *))
{
	vector new = newVectorWithCapacity(vectorSize(v));
	for (size_t i = 0; i < vectorSize(v); i++)
		vectorAdd(&new, dupData(vectorAt(v, i)));
	return new;
}

vector vectorCloneRange(const vector *v, ssize_t start, ssize_t end, const ssize_t step, void *(*dupData)(const void *));

void vectorShrinkToFit(vector *v)
{
	v->capacity = v->size;
	v->data = realloc(v->data, sizeof(void *) * v->capacity);
}

void vectorClear(vector *v, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < vectorSize(v); i++)
			freeData(vectorAt(v, i));
	v->size = 0;
}

void vectorFree(vector *v, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < vectorSize(v); i++)
			freeData(vectorAt(v, i));
	free(v->data);
}

void vectorSort(vector *v, int (*cmp)(const void *, const void *))
{
	qsort(v->data, v->size, sizeof(void *), cmp);
}

void vectorReverse(vector *v)
{
	size_t start = 0;
	size_t end = vectorSize(v) - 1;

	while (start < end) {
		swap(&v->data[start], &v->data[end], sizeof(void *));
		start++;
		end--;
	}
}

void vectorShuffle(vector *v);

void vectorReduce(vector *v, bool (*shouldRemove)(const void *, size_t i));

void vectorTransform(vector *v, void *(*transform)(void *));
void vectorTransformRange(vector *v, void *(*transform)(void *), ssize_t start, ssize_t end, const size_t step);

void vectorForEach(const vector *v, void (*action)(const void *))
{
	for (size_t i = 0; i < vectorSize(v); i++)
		action(vectorAt(v, i));
}

void vectorForEachRange(const vector *v, ssize_t start, ssize_t end, const size_t step, void (*action)(const void *));

void vectorPrint(const vector *v, void (*printData)(const void *))
{
	printf("[ ");
	vectorForEach(v, printData);
	printf(" ]");
}
