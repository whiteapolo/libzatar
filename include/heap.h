#ifndef HEAP_H
#define HEAP_H
#include "shared.h"

#define MIN_HEAP_CAPACITY 16

typedef struct {
	void **vec;
	size_t size;
	size_t capacity;
	int (*cmp)(const void *, const void *);
} Heap;

Heap *newHeap(int cmp)(const void *, const void *));
Heap  *newHeapWithCapacity(int cmp(const void *, const void *), const size_t initialCapacity);
void heapPush(Heap *h, void *data);
const void *heapPeek(const Heap *h);
void *heapPop(Heap *h);
size_t heapGetSize(const Heap *h);
bool heapIsEmpty(const Heap *h);
void heapShrinkToFit(Heap *h);
void heapClear(Heap *h, void freeData(void *));
void heapFree(Heap *h, void freeData(void *));

#endif
