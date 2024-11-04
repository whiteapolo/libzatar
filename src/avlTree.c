#include "avlTree.h"

avlNode *newAvlNode(void *key, void *data)
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
	node->height = 1 + zatarMax(getHeight(node->right), getHeight(node->left));
}

int getBalanceFactor(const avlNode *node)
{
	if (node == NULL)
		return 0;
	return getHeight(node->left) - getHeight(node->right);
}

avlNode *avlGetMin(avlNode *root)
{
	while (root->left != NULL)
		root = root->left;
	return root;
}

avlNode *avlGetMax(avlNode *root)
{
	while (root->right != NULL)
		root = root->right;
	return root;
}

void leftRotate(avlNode **root)
{
	avlNode *newRoot = (*root)->right;
	avlNode *tmp = NULL;
	if (newRoot != NULL)
		tmp = newRoot->left;

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

	// avlNode *tmp = NULL;
	// if (newRoot != NULL)
	// 	tmp = newRoot->left;

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

const avlNode *avlFindNode(const avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *))
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
	return avlFindNode(root, key, cmpKeys) != NULL;
}

void *avlFind(const avlNode *root, const void *key, int (*cmpKeys)(const void *,const void *))
{
	const avlNode *node = avlFindNode(root, key, cmpKeys);
	if (node != NULL)
		return node->data;
	return NULL;
}

void avlInsert(avlNode **root, void *key, void *data, int (*cmpKeys)(const void *,const void *))
{
	if (*root == NULL) {
		*root = newAvlNode(key, data);
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

		avlNode *succesor = avlGetMin((*root)->right);

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
	avlNode *node = (avlNode *)avlFindNode(root, key, cmpKeys);

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
