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

void heapifyMaxDown(heap *h, size_t i)
{

}

void heapifyMinDown(heap *h, size_t i)
{

}

void heapPush(heap *h, void *data)
{
	const size_t i = (h->size++);
	const size_t p = parent(i);

	h->vec[i] = data;

	if (i == 0)
		return;

	if (isMinLevel(h->size)) {
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

const void *heapPeek(const heap *h);
void *heapPop(heap *h);
size_t heapGetSize(const heap *h);
bool heapIsEmpty(const heap *h);
void heapShrinkToFit(heap *h);
void heapClear(heap *h, void (*freeData)(void *));
void heapFree(heap *h, void (*freeData)(void *));
