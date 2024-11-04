#include "map.h"
#include "avlTree.h"

map newMap(int (*cmpKeys)(const void *, const void *))
{
	map m;
	m.cmpKeys = cmpKeys;
	m.root = NULL;
	return m;
}

map newMapFrom(int (*cmpKeys)(const void *, const void *), void *key, void *data, ...)
{
	map m = newMap(cmpKeys);
	mapInsert(&m, key, data);
	va_list ap;
	va_start(ap, data);
	mapInsertAllVa(&m, ap);
	va_end(ap);
	return m;
}

void mapInsert(map *m, void *key, void *data)
{
	avlInsert(&m->root, key, data, m->cmpKeys);
}

void mapInsertAllVa(map *m, va_list ap)
{
	void *key = va_arg(ap, void *);
	void *data = va_arg(ap, void *);

	while (key && data) {
		mapInsert(m, key, data);
		key = va_arg(ap, void *);
		data = va_arg(ap, void *);
	}
}

// need to be terminated with NULL, NULL
void mapInsertAll(map *m, ...)
{
	va_list ap;
	va_start(ap, m);
	mapInsertAllVa(m, ap);
	va_end(ap);
}

void *mapFind(const map *m, const void *key)
{
	return avlFind(m->root, key, m->cmpKeys);
}

bool mapIsExists(const map *m, const void *key)
{
	return avlIsExists(m->root, key, m->cmpKeys);
}

void mapRemove(map *m, const void *key, void (*freeKey)(void *), void (*freeData)(void *))
{
	avlRemove(&m->root, key, m->cmpKeys, freeKey, freeData);
}

void mapUpdate(map m, const void *key, void (*freeData)(void *), void *newData)
{
	avlUpdate(m.root, key, m.cmpKeys, freeData, newData);
}

void mapOrderTraverse(const map *m, void (*action)(const void *key, const void *data, void *arg), void *arg)
{
    avlOrderTraverse(m->root, action, arg);
}

bool mapIsEmpty(const map *m)
{
    return m->root == NULL;
}

void mapFree(map *m, void (*freeKey)(void *), void (*freeData)(void *))
{
	avlFree(m->root, freeKey, freeData);
}
