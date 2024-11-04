#ifndef AVL_TREE_H
#define AVL_TREE_H
#include "shared.h"

typedef struct avlNode {
	struct avlNode *left;
	struct avlNode *right;
	void *key;
	void *data;
	int height;
} avlNode;

avlNode *newAvlNode(void *key, void *data);
avlNode *avlGetMin(avlNode *root);
avlNode *avlGetMax(avlNode *root);
const avlNode *avlFindNode(const avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *));
bool avlIsExists(const avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *));
void *avlFind(const avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *));
void avlInsert(avlNode **root, void *key, void *data, int (*cmpKeys)(const void *,const void *));
void avlRemove(avlNode **root, const void *key, int (*cmpKeys)(const void *,const void *), void (*freeKey)(void *), void (*freeData)(void *));
void avlUpdate(avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *), void (*freeData)(void *), void *newData);
void avlOrderTraverse(const avlNode *root, void (*action)(const void *key, const void *data, void *arg), void *arg);
void avlFree(avlNode *root, void (*freeKey)(void *), void (*freeData)(void *));

#endif
