#ifndef MAP_H
#define MAP_H

#define $(element) (&((typeof(element)){element}))
#define DUP_VAL(value) memdup($(value), sizeof(value))

typedef struct avlNode {
	struct avlNode *left;
	struct avlNode *right;
	char *key;
	void *data;
	int height;
} avlNode;

typedef avlNode *map;

map newMap();

// need to be terminated with NULL, NULL
map newMapFrom(const char *key, void *data, ...);

void mapInsert(map *m, const char *key, void *data);

// need to be terminated with NULL, NULL
void mapInsertAllAp(map *m, va_list ap);

// need to be terminated with NULL, NULL
void mapInsertAll(map *m, ...);

const void *mapFind(const map m, const char *key);

bool mapIsExists(const map m, const char *key);

void mapRemove(map *m, const char *key, void (*freeData)(void *));

void mapFree(map m, void (*freeData)(void *));

void mapUpdate(map m, const char *key,
		       void (*freeData)(void *), void *newData);

// util
void *memdup(const void *mem, int size);

#ifdef MAP_IMPL

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int avlMax(int a, int b)
{
	return a > b ? a : b;
}

avlNode *makeNode(const char *key, void *data)
{
	avlNode *n = malloc(sizeof(avlNode));
	n->key = strdup(key);
	n->data = data;
	n->height = 1;
	n->left = NULL;
	n->right = NULL;
	return n;
}

int getHeight(avlNode *node)
{
	if (node == NULL)
		return 0;
	return node->height;
}

void updateHeight(avlNode *node)
{
	node->height = 1 + avlMax(getHeight(node->right), getHeight(node->left));
}

int getBalanceFactor(avlNode *node)
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

const avlNode *findAvlNode(const avlNode *root, const char *key)
{
	int cmpRes;
	while (root && (cmpRes = strcmp(key, root->key))) {
		if (cmpRes > 0)
			root = root->right;
		else
			root = root->left;
	}
	return root;
}

bool avlIsExists(const avlNode *root, const char *key)
{
	return findAvlNode(root, key) != NULL;
}

const void *avlFind(const avlNode *root, const char *key)
{
	const avlNode *node = findAvlNode(root, key);
	if (node != NULL)
		return node->data;
	return NULL;
}

void avlInsert(avlNode **root, const char *key, void *data)
{
	if (*root == NULL)
		return (void)(*root = makeNode(key, data));

	else if (strcmp(key, (*root)->key) > 0)
		avlInsert(&(*root)->right, key, data);

	else
		avlInsert(&(*root)->left, key, data);


	updateHeight(*root);
	int bf = getBalanceFactor(*root);
	int cmpRes = strcmp(key, (*root)->key);


	if (bf > 1 && cmpRes < 0)
		rightRotate(root);

	else if (bf < -1 && cmpRes > 0)
		leftRotate(root);

	else if (bf > 1 && cmpRes > 0)
		leftRightRotate(root);

	else if (bf < -1 && cmpRes < 0)
		rightLeftRotate(root);
}

void avlRemove(avlNode **root, const char *key, void (*freeData)(void *))
{
	if (*root == NULL) {
		return;

	} else if (strcmp(key, (*root)->key) > 0) {
		return avlRemove(&((*root)->right), key, freeData);

	} else if (strcmp(key, (*root)->key) < 0) {
		return avlRemove(&((*root)->left), key, freeData);

	} else {
		if (freeData)
			freeData((*root)->data);

		if ((*root)->left == NULL) {
			avlNode *tmp = (*root)->right;
			free((*root)->key);
			free(*root);
			*root = tmp;
			return;
		} else if ((*root)->right == NULL) {
			avlNode *tmp = (*root)->left;
			free((*root)->key);
			free(*root);
			*root = tmp;
			return;
		}

		avlNode *succesor = getMinNode((*root)->right);

		free((*root)->key);
		(*root)->key = strdup(succesor->key);
		(*root)->data = succesor->data;

		avlRemove(&((*root)->right), succesor->key, NULL);
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

	return;
}

void avlUpdate(avlNode *root, const char *key, void (*freeData)(void *), void *newData)
{
	avlNode *node = (avlNode *)findAvlNode(root, key);

	if (node == NULL)
		return;

	if (freeData)
		freeData(node->data);

	node->data = newData;
}

void avlFree(avlNode *root, void (*freeData)(void *))
{
	if (root == NULL)
		return;

	free(root->key);

	if (freeData != NULL)
		freeData(root->data);

	avlFree(root->left, freeData);
	avlFree(root->right, freeData);

	free(root);
}

map newMap()
{
	return NULL;
}

map newMapFrom(const char *key, void *data, ...)
{
	map m = newMap();
	mapInsert(&m, key, data);
	va_list ap;
	va_start(ap, data);
	mapInsertAllAp(&m, ap);
	va_end(ap);
	return m;
}

void mapInsert(map *m, const char *key, void *data)
{
	avlInsert(m, key, data);
}

void mapInsertAllAp(map *m, va_list ap)
{
	const char *key = va_arg(ap, const char *);
	void *data = va_arg(ap, void *);

	while (key && data) {
		mapInsert(m, key, data);
		key = va_arg(ap, const char *);
		data = va_arg(ap, void *);
	}
}

// need to be terminated with NULL, NULL
void mapInsertAll(map *m, ...)
{
	va_list ap;
	va_start(ap, m);
	mapInsertAllAp(m, ap);
	va_end(ap);
}

const void *mapFind(const map m, const char *key)
{
	return avlFind(m, key);
}

bool mapIsExists(const map m, const char *key)
{
	return avlIsExists(m, key);
}

void mapRemove(map *m, const char *key, void (*freeData)(void *))
{
	return avlRemove(m, key, freeData);
}

void mapUpdate(map m, const char *key, 
		       void (*freeData)(void *), void *newData)
{
	avlUpdate(m, key, freeData, newData);
}

void mapFree(map m, void (*freeData)(void *))
{
	avlFree(m, freeData);
}

void *memdup(const void *mem, int size)
{
	void *newMem = malloc(size);
	memcpy(newMem, mem, size);
	return newMem;
}

#endif

#endif
