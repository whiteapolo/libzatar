#ifndef MIN_MAX_HEAP_H
#define MIN_MAX_HEAP_H
#include "shared.h"

#define MIN_MIN_MAX_HEAP_CAPACITY 16

typedef struct {
	void **vec;
	size_t size;
	size_t capacity;
	int (*cmp)(const void *, const void *);
} minMaxHeap;

minMaxHeap newMinMaxHeap(int (*cmp)(const void *, const void *));
minMaxHeap newMinMaxHeapWithCapacity(int (*cmp)(const void *, const void *), const size_t initialCapacity);

void minMaxHeapPush(minMaxHeap *h, void *data);
const void *minMaxHeapPeek(const minMaxHeap *h);
void *minMaxHeapPop(minMaxHeap *h);
size_t minMaxHeapGetSize(const minMaxHeap *h);
bool minMaxHeapIsEmpty(const minMaxHeap *h);
void minMaxHeapShrinkToFit(minMaxHeap *h);
void minMaxHeapClear(minMaxHeap *h, void (*freeData)(void *));
void minMaxHeapFree(minMaxHeap *h, void (*freeData)(void *));

#endif
