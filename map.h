#ifndef MAP_H
#define MAP_H
#include <stdarg.h>

#define ADDRESS(element) (&((typeof(element)){element}))
#define DUP(value) memdup(ADDRESS(value), sizeof(value))

typedef int (*cmpKeysType)(const void *, const void *);

typedef struct avlNode {
	struct avlNode *left;
	struct avlNode *right;
	void *key;
	void *data;
	int height;
} avlNode;

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

void *memdup(const void *mem, const size_t size);

#ifdef MAP_IMPL
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int avlMax(const int a, const int b)
{
	return a > b ? a : b;
}

avlNode *makeNode(void *key, void *data)
{
	avlNode *n = malloc(sizeof(avlNode));
	n->key = key;
	n->data = data;
	n->height = 1;
	n->left = NULL;
	n->right = NULL;
	return n;
}

int getHeight(const avlNode *node)
{
	if (node == NULL)
		return 0;
	return node->height;
}

void updateHeight(avlNode *node)
{
	node->height = 1 + avlMax(getHeight(node->right), getHeight(node->left));
}

int getBalanceFactor(const avlNode *node)
{
	if (node == NULL)
		return 0;
	return getHeight(node->left) - getHeight(node->right);
}

avlNode *getMinNode(avlNode *root)
{
	while (root->left != NULL)
		root = root->left;
	return root;
}

avlNode *getMaxNode(avlNode *root)
{
	while (root->right != NULL)
		root = root->right;
	return root;
}

void leftRotate(avlNode **root)
{
	avlNode *newRoot = (*root)->right;
	avlNode *tmp = newRoot->left;

	newRoot->left = *root;
	(*root)->right = tmp;

	updateHeight(newRoot->left);
	updateHeight(newRoot);

	*root = newRoot;
}

void rightRotate(avlNode **root)
{
	avlNode *newRoot = (*root)->left;
	avlNode *tmp = newRoot->right;

	newRoot->right = *root;
	(*root)->left = tmp;

	updateHeight(newRoot->right);
	updateHeight(newRoot);

	*root = newRoot;
}

void leftRightRotate(avlNode **root)
{
	leftRotate(&(*root)->left);
	rightRotate(root);
}

void rightLeftRotate(avlNode **root)
{
	rightRotate(&(*root)->right);
	leftRotate(root);
}

const avlNode *findAvlNode(const avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *))
{
	int cmpRes;
	while (root && (cmpRes = cmpKeys(key, root->key))) {
		if (cmpRes > 0)
			root = root->right;
		else
			root = root->left;
	}
	return root;
}

bool avlIsExists(const avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *))
{
	return findAvlNode(root, key, cmpKeys) != NULL;
}

void *avlFind(const avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *))
{
	const avlNode *node = findAvlNode(root, key, cmpKeys);
	if (node != NULL)
		return node->data;
	return NULL;
}

void avlInsert(avlNode **root, void *key, void *data, int (*cmpKeys)(const void *,const void *))
{
	if (*root == NULL) {
		*root = makeNode(key, data);
		return;
	} else if (cmpKeys(key, (*root)->key) > 0) {
		avlInsert(&(*root)->right, key, data, cmpKeys);
	} else {
		avlInsert(&(*root)->left, key, data, cmpKeys);
	}

	updateHeight(*root);
	const int bf = getBalanceFactor(*root);
	const int cmpRes = cmpKeys(key, (*root)->key);

	if (bf > 1 && cmpRes < 0)
		rightRotate(root);
	else if (bf < -1 && cmpRes > 0)
		leftRotate(root);
	else if (bf > 1 && cmpRes > 0)
		leftRightRotate(root);
	else if (bf < -1 && cmpRes < 0)
		rightLeftRotate(root);
}

void avlRemove(avlNode **root, const void *key, int (*cmpKeys)(const void *,const void *), void (*freeKey)(void *), void (*freeData)(void *))
{
	if (*root == NULL) {
		return;
	} else if (cmpKeys(key, (*root)->key) > 0) {
		avlRemove(&((*root)->right), key, cmpKeys, freeKey, freeData);
		return;
	} else if (cmpKeys(key, (*root)->key) < 0) {
		avlRemove(&((*root)->left), key, cmpKeys, freeKey, freeData);
		return;
	} else {
		if (freeKey)
			freeKey((*root)->key);

		if (freeData)
			freeData((*root)->data);

		if ((*root)->left == NULL) {
			avlNode *tmp = (*root)->right;
			free(*root);
			*root = tmp;
			return;
		} else if ((*root)->right == NULL) {
			avlNode *tmp = (*root)->left;
			free(*root);
			*root = tmp;
			return;
		}

		avlNode *succesor = getMinNode((*root)->right);

		(*root)->key = succesor->key;
		(*root)->data = succesor->data;

		avlRemove(&((*root)->right), succesor->key, cmpKeys, NULL, NULL);
	}

	updateHeight(*root);
	int bf = getBalanceFactor(*root);

	if (bf > 1 && getBalanceFactor((*root)->left) >= 0)
		rightRotate(root);
	else if (bf < -1 && getBalanceFactor((*root)->right) <= 0)
		leftRotate(root);
	else if (bf > 1 && getBalanceFactor((*root)->left) < 0)
		leftRightRotate(root);
	else if (bf < -1 && getBalanceFactor((*root)->right) > 0)
		rightLeftRotate(root);
}

void avlUpdate(avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *), void (*freeData)(void *), void *newData)
{
	avlNode *node = (avlNode *)findAvlNode(root, key, cmpKeys);

	if (node == NULL)
		return;

	if (freeData)
		freeData(node->data);

	node->data = newData;
}

void avlOrderTraverse(const avlNode *root, void (*action)(const void *key, const void *data, void *arg), void *arg)
{
	if (root == NULL)
		return;

    avlOrderTraverse(root->left, action, arg);
    action(root->key, root->data, arg);
    avlOrderTraverse(root->right, action, arg);
}

void avlFree(avlNode *root, void (*freeKey)(void *), void (*freeData)(void *))
{
	if (root == NULL)
		return;

	if (freeKey != NULL)
		freeKey(root->key);

	if (freeData != NULL)
		freeData(root->data);

	avlFree(root->left, freeKey, freeData);
	avlFree(root->right, freeKey, freeData);

	free(root);
}

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

#ifndef MEMDUP
#define MEMDUP
void *memdup(const void *mem, const size_t size)
{
	void *newMem = malloc(size);
	memcpy(newMem, mem, size);
	return newMem;
}
#endif
#endif
#endif
