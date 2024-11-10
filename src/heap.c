#include "heap.h"

size_t parent(size_t i);
size_t right(size_t i);
size_t left(size_t i);
void heapifyUp(heap *h, size_t i);
void heapifyDown(heap *h, size_t i);

heap newHeapWithCapacity(int (*cmp)(const void *, const void *), const size_t initialCapacity)
{
	heap h;
	h.size = 0;
	h.capacity = initialCapacity;
	h.vec = malloc(sizeof(void*) * h.capacity);
	h.cmp = cmp;
	return h;
}

heap newHeap(int (*cmp)(const void *, const void *))
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

void heapifyUp(heap *h, size_t i)
{
	if (i == 0)
		return;

	if (h->cmp(h->vec[parent(i)], h->vec[i]) > 0) {
		swap(&h->vec[i], &h->vec[parent(i)], sizeof(void*));
		heapifyUp(h, parent(i));
	}
}

void heapifyDown(heap *h, size_t i)
{
	size_t l = left(i);
	size_t r = right(i);

	size_t largest = i;

	if (l < h->size && h->cmp(h->vec[i], h->vec[l]) > 0)
		largest = l;

	if (r < h->size && h->cmp(h->vec[largest], h->vec[r]) > 0)
		largest = r;

	if (largest != i) {
		swap(&h->vec[i], &h->vec[largest], sizeof(void*));
		heapifyDown(h, largest);
	}
}

void heapPush(heap *h, void *data)
{
	h->size++;
	if (h->size > h->capacity) {
		h->capacity *= 2;
		h->vec = realloc(h->vec, sizeof(void*) * h->capacity);
	}
	h->vec[h->size - 1] = data;
	heapifyUp(h, h->size - 1);
}

const void *heapPeek(const heap *h)
{
	return h->vec[0];
}

void *heapPop(heap *h)
{
	void *ret = h->vec[0];
	h->vec[0] = h->vec[--h->size];
	heapifyDown(h, 0);
	return ret;
}

size_t heapGetSize(const heap *h)
{
	return h->size;
}

bool heapIsEmpty(const heap *h)
{
	return heapGetSize(h) == 0;
}

void heapShrinkToFit(heap *h)
{
	h->capacity = h->size;
	h->vec = realloc(h->vec, sizeof(void*) * h->capacity);
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
