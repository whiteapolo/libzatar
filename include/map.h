#ifndef MAP_H
#define MAP_H
#include "shared.h"
#include "avlTree.h"

typedef int (*cmpKeysType)(const void *, const void *);

typedef struct {
	avlNode *root;
	int (*cmpKeys)(const void *, const void *);
} map;

map newMap(int (*cmpKeys)(const void *, const void *));
map newMapFrom(int (*cmpKeys)(const void *, const void *), void *key, void *data, ...);
void mapInsert(map *m, void *key, void *data);
void mapInsertAllVa(map *m, va_list ap);
// need to be terminated with NULL, NULL
void mapInsertAll(map *m, ...);
void *mapFind(const map *m, const void *key);
bool mapIsExists(const map *m, const void *key);
void mapRemove(map *m, const void *key, void (*freeKey)(void *), void (*freeData)(void *));
void mapUpdate(map m, const void *key, void (*freeData)(void *), void *newData);
void mapOrderTraverse(const map *m, void (*action)(const void *key, const void *data, void *arg), void *arg);
void mapFree(map *m, void (*freeKey)(void *), void (*freeData)(void *));
bool mapIsEmpty(const map *m);

#endif
