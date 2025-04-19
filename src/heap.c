#include "heap.h"

size_t parent(size_t i);
size_t right(size_t i);
size_t left(size_t i);
void heapifyUp(Heap *h, size_t i);
void heapifyDown(Heap *h, size_t i);

Heap *newHeapWithCapacity(int cmp(const void *, const void *), const size_t initialCapacity)
{
	Heap *h = malloc(sizeof(Heap));
	h->size = 0;
	h->capacity = initialCapacity;
	h->vec = malloc(sizeof(void*) * h.capacity);
	h->cmp = cmp;

	return h;
}

Heap *newHeap(int cmp(const void *, const void *))
{
	return newHeapWithCapacity(cmp, MIN_HEAP_CAPACITY);
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

void heapifyUp(Heap *h, size_t i)
{
	if (i == 0) {
		return;
	}

	if (h->cmp(h->vec[parent(i)], h->vec[i]) > 0) {
		swap(&h->vec[i], &h->vec[parent(i)], sizeof(void*));
		heapifyUp(h, parent(i));
	}
}

void heapifyDown(Heap *h, size_t i)
{
	size_t l = left(i);
	size_t r = right(i);

	size_t largest = i;

	if (l < h->size && h->cmp(h->vec[i], h->vec[l]) > 0) {
		largest = l;
	}

	if (r < h->size && h->cmp(h->vec[largest], h->vec[r]) > 0) {
		largest = r;
	}

	if (largest != i) {
		swap(&h->vec[i], &h->vec[largest], sizeof(void*));
		heapifyDown(h, largest);
	}
}

void heapPush(Heap *h, void *data)
{
	h->size++;

	if (h->size > h->capacity) {
		h->capacity *= 2;
		h->vec = realloc(h->vec, sizeof(void*) * h->capacity);
	}

	h->vec[h->size - 1] = data;
	heapifyUp(h, h->size - 1);
}

const void *heapPeek(const Heap *h)
{
	return h->vec[0];
}

void *heapPop(Heap *h)
{
	void *ret = h->vec[0];
	h->vec[0] = h->vec[--h->size];
	heapifyDown(h, 0);

	return ret;
}

size_t heapSize(const Heap *h)
{
	return h->size;
}

bool heapIsEmpty(const Heap *h)
{
	return heapSize(h) == 0;
}

void heapShrinkToFit(Heap *h)
{
	h->capacity = h->size;
	h->vec = realloc(h->vec, sizeof(void*) * h->capacity);
}

void heapClear(Heap *h, void freeData(void *))
{
	if (freeData) {
		for (size_t i = 0; i < h->size; i++) {
			freeData(h->vec[i]);
		}
	}

	h->size = 0;
}

void heapFree(Heap *h, void freeData(void *))
{
	heapClear(h, freeData);
	free(h->vec);
	free(h);
}
