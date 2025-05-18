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

bool avl_is_exists(avlNode *root, void *key, int cmpKeys(void *,void *));
void *avl_find(avlNode *root, void *key, int cmpKeys(void *,void *));
void avl_insert(avlNode **root, void *key, void *data, int cmpKeys(void *,void *));
void avl_remove(avlNode **root, void *key, int cmpKeys(void *,void *), void freeKey(void *), void freeData(void *));
void avl_update(avlNode *root, void *key, int cmpKeys(void *,void *), void freeData(void *), void *newData);
void avl_order_traverse(avlNode *root, void action(void *key, void *data, void *arg), void *arg);
void avl_print(avlNode *root, void print(void *key, void *data, void *arg), void *arg, int padding);
void avl_free(avlNode *root, void freeKey(void *), void freeData(void *));

#endif
