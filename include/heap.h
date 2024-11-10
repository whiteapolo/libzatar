#ifndef HEAP_H
#define HEAP_H
#include "shared.h"

#define MIN_HEAP_CAPACITY 16

typedef struct {
	void **vec;
	size_t size;
	size_t capacity;
	int (*cmp)(const void *, const void *);
} heap;

heap newHeap(int (*cmp)(const void *, const void *));
heap newHeapWithCapacity(int (*cmp)(const void *, const void *), const size_t initialCapacity);
void heapPush(heap *h, void *data);
const void *heapPeek(const heap *h);
void *heapPop(heap *h);
size_t heapGetSize(const heap *h);
bool heapIsEmpty(const heap *h);
void heapShrinkToFit(heap *h);
void heapClear(heap *h, void (*freeData)(void *));
void heapFree(heap *h, void (*freeData)(void *));

#endif
