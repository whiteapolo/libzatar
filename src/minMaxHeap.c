#include "minMaxHeap.h"
#include "shared.h"
#include <math.h>

size_t right(size_t i);
size_t left(size_t i);
size_t nodeLevel(size_t i);
size_t isMinLevel(size_t i);

void minMaxHeapifyMaxUp(minMaxHeap *h, size_t i);
void minMaxHeapifyMinUp(minMaxHeap *h, size_t i);
void minMaxHeapifyMaxDown(minMaxHeap *h, size_t i);
void minMaxHeapifyMinDown(minMaxHeap *h, size_t i);

minMaxHeap newMinMaxminMaxHeapWithCapacity(int (*cmp)(const void *, const void *), const size_t initialCapacity)
{
	minMaxHeap h;
	h.vec = malloc(sizeof(void *) * initialCapacity);
	h.size = 0;
	h.capacity = initialCapacity;
	h.cmp = cmp;
	return h;
}

minMaxHeap newMinMaxminMaxHeap(int (*cmp)(const void *, const void *))
{
	return newMinMaxminMaxHeapWithCapacity(cmp, MIN_MIN_MAX_HEAP_CAPACITY);
}

size_t parent(size_t i)
{
	return i / 2;
}

size_t right(size_t i)
{
	return (i * 2) + 2;
}

size_t left(size_t i)
{
	return (i * 2) + 1;
}

size_t nodeLevel(size_t i)
{
	return log2(i);
}

size_t isMinLevel(size_t i)
{
	return nodeLevel(i) % 2 == 0;
}

bool hasGrandparent(size_t i)
{
	return i > 2;
}

size_t getMaxIndex(const minMaxHeap *h)
{
	if (h->size == 1)
		return 0;
	if (h->size == 2)
		return 1;
	if (h->cmp(h->vec[1], h->vec[2]) > 0)
		return 1;
	return 2;
}

void minMaxHeapifyMaxUp(minMaxHeap *h, size_t i)
{
	const size_t grandparent = parent(parent(i));
	if (hasGrandparent(i) && h->cmp(h->vec[i], h->vec[grandparent]) > 0) {
		swap(&h->vec[i], &h->vec[grandparent], sizeof(void *));
		minMaxHeapifyMaxUp(h, grandparent);
	}
}

void minMaxHeapifyMinUp(minMaxHeap *h, size_t i)
{
	const size_t grandparent = parent(parent(i));
	if (hasGrandparent(i) && h->cmp(h->vec[i], h->vec[grandparent]) < 0) {
		swap(&h->vec[i], &h->vec[grandparent], sizeof(void *));
		minMaxHeapifyMinUp(h, grandparent);
	}
}

void minMaxHeapifyUp(minMaxHeap *h, size_t i)
{
	const size_t p = parent(i);

	if (i == 0)
		return;

	if (isMinLevel(i)) {
		if (h->cmp(h->vec[i], h->vec[p]) > 0) {
			swap(&h->vec[i], &h->vec[p], sizeof(void *));
			minMaxHeapifyMaxUp(h, p);
		} else {
			minMaxHeapifyMinUp(h, i);
		}
	} else {
		if (h->cmp(h->vec[i], h->vec[p]) < 0) {
			swap(&h->vec[i], &h->vec[p], sizeof(void *));
			minMaxHeapifyMinUp(h, p);
		} else {
			minMaxHeapifyMaxUp(h, i);
		}
	}
}

void minMaxHeapifyMaxDown(minMaxHeap *h, size_t i)
{

}

void minMaxHeapifyMinDown(minMaxHeap *h, size_t i)
{

}

void minMaxHeapifyDown(minMaxHeap *h, size_t i)
{
	if (isMinLevel(i))
		minMaxHeapifyMinDown(h, i);
	else
		minMaxHeapifyMaxDown(h, i);
}

void minMaxHeapPush(minMaxHeap *h, void *data)
{
	h->size++;
	if (h->size > h->capacity) {
		h->capacity *= 2;
		h->vec = realloc(h->vec, sizeof(void *) * h->capacity);
	}

	h->vec[h->size - 1] = data;
	minMaxHeapifyUp(h, h->size - 1);


}

const void *minMaxHeapPeekMin(const minMaxHeap *h)
{
	return h->vec[0];
}

const void *minMaxHeapPeekMax(const minMaxHeap *h)
{
	return h->vec[getMaxIndex(h)];
}

void *minMaxHeapPopMax(minMaxHeap *h)
{
	size_t minI = getMaxIndex(h);
	void *minData = h->vec[minI];
	h->vec[minI] = h->vec[--h->size];
	minMaxHeapifyDown(h, minI);
}

void *minMaxHeapPopMin(minMaxHeap *h)
{
	void *minData = h->vec[0];
	h->vec[0] = h->vec[--h->size];
	minMaxHeapifyMinDown(h, 0);
	return minData;
}

size_t minMaxHeapGetSize(const minMaxHeap *h)
{
	return h->size;
}

bool minMaxHeapIsEmpty(const minMaxHeap *h)
{
	return h->size == 0;
}

void minMaxHeapShrinkToFit(minMaxHeap *h)
{
	h->capacity = h->size;
	h->vec = realloc(h->vec, sizeof(void *) * h->capacity);
}

void minMaxHeapClear(minMaxHeap *h, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < h->size; i++)
			freeData(h->vec[i]);
	h->size = 0;
}

void minMaxHeapFree(minMaxHeap *h, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < h->size; i++)
			freeData(h->vec[i]);
	free(h->vec);
}
