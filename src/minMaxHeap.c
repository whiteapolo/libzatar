#include "minMaxHeap.h"
#include "shared.h"

size_t right(size_t i);
size_t left(size_t i);
size_t nodeLevel(size_t i);
size_t isMinLevel(size_t i);

void heapifyMaxUp(heap *h, size_t i);
void heapifyMinUp(heap *h, size_t i);
void heapifyMaxDown(heap *h, size_t i);
void heapifyMinDown(heap *h, size_t i);

minMaxHeap newMinMaxHeapWithCapacity(int (*cmp)(const void *, const void *), const size_t initialCapacity)
{
	minMaxHeap h;
	h.vec = malloc(sizeof(void *) * initialCapacity);
	h.size = 0;
	h.capacity = initialCapacity;
	h.cmp = cmp;
	return h;
}

minMaxHeap newMinMaxHeap(int (*cmp)(const void *, const void *))
{
	return newMinMaxHeapWithCapacity(cmp, MIN_MIN_MAX_HEAP_CAPACITY);
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

}

size_t isMinLevel(size_t i)
{
	return nodeLevel(i) % 2 == 0;
}

bool hasGrandparent(size_t i)
{
	return i > 2;
}

size_t getMaxIndex(const heap *h)
{
	if (h->size == 1)
		return 0;
	if (h->size == 2)
		return 1;
	if (h->cmp(h->vec[1], h->vec[2]) > 0)
		return 1;
	return 2;
}

void heapifyMaxUp(heap *h, size_t i)
{
	const size_t grandparent = parent(parent(i));
	if (hasGrandparent(i) && h->cmp(h->vec[i], h->vec[grandparent]) > 0) {
		swap(&h->vec[i], &h->vec[grandparent], sizeof(void *));
		heapifyMaxUp(h, grandparent);
	}
}

void heapifyMinUp(heap *h, size_t i)
{
	const size_t grandparent = parent(parent(i));
	if (hasGrandparent(i) && h->cmp(h->vec[i], h->vec[grandparent]) < 0) {
		swap(&h->vec[i], &h->vec[grandparent], sizeof(void *));
		heapifyMinUp(h, grandparent);
	}
}

void heapifyUp(heap *h, size_t i)
{
	const size_t p = parent(i);

	if (i == 0)
		return;

	if (isMinLevel(i)) {
		if (h->cmp(h->vec[i], h->vec[p] > 0)) {
			swap(&h->vec[i], &h->vec[p], sizeof(void *));
			heapifyMaxUp(h, p)
		} else {
			heapifyMinUp(h, i);
		}
	} else {
		if (h->cmp(h->vec[i], h->vec[p] < 0)) {
			swap(&h->vec[i], &h->vec[p], sizeof(void *));
			heapifyMinUp(h, p)
		} else {
			heapifyMaxUp(h, i);
		}
	}
}

void heapifyMaxDown(heap *h, size_t i)
{

}

void heapifyMinDown(heap *h, size_t i)
{

}

void heapifyDown(heap *h, size_t i)
{
	if (isMinLevel(i))
		heapifyMinDown(h, i);
	else
		heapifyMaxDown(h, i);
}

void heapPush(heap *h, void *data)
{
	h->size++;
	if (h->size > h->capacity) {
		h->capacity *= 2;
		h->vec = realloc(h->vec, sizeof(void *) * h->capacity);
	}

	h->vec[h->size - 1] = data;
	heapifyUp(h, h->size - 1);


}

const void *heapPeekMin(const heap *h)
{
	return h->vec[0];
}

const void *heapPeekMax(const heap *h)
{
	return h->vec[getMaxIndex(h)];
}

void *heapPopMax(heap *h)
{
	size_t minI = getMaxIndex(h);
	void *minData = h->vec[minI];
	h->vec[minI] = h->vec[--h->size];
	heapifyDown(h, minI);
}

void *heapPopMin(heap *h)
{
	void *minData = h->vec[0];
	h->vec[0] = h->vec[--h->size];
	heapifyMinDown(h, 0);
	return minData;
}

size_t heapGetSize(const heap *h)
{
	return h->size;
}

bool heapIsEmpty(const heap *h)
{
	return h->size == 0;
}

void heapShrinkToFit(heap *h)
{
	h->capacity = h->size;
	h->vec = realloc(h->vec, sizeof(void *) * h->capacity);
}

void heapClear(heap *h, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < h->size; i++)
			freeData(h->vec[i]);
	h->size = 0;
}

void heapFree(heap *h, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (size_t i = 0; i < h->size; i++)
			freeData(h->vec[i]);
	free(h->vec);
}
