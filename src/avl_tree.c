#include "avl_tree.h"

#define AVL_MAX(a, b) ((a) > (b) ? (a) : (b))

#define AVL_NODE_GET_HEIGHT(node) \
	(node ? node->height : 0)

#define AVL_NODE_UPDATE_HEIGHT(node)                  \
	do {                                              \
		(node)->height = 1 + AVL_MAX(                 \
				AVL_NODE_GET_HEIGHT((node)->right),   \
				AVL_NODE_GET_HEIGHT((node)->left)     \
		);                                            \
	} while (0)

#define AVL_NODE_GET_BALANCE_FACTOR(node)                                   \
	((node) ?                                                               \
	 AVL_NODE_GET_HEIGHT((node)->left) - AVL_NODE_GET_HEIGHT((node)->right) \
	 : 0)

#define AVL_NODE_LEFT_ROTATE(root)                 \
	do {                                           \
		avlNode *newRoot = (*(root))->right;       \
		avlNode *tmp = newRoot->left;              \
                                                   \
		newRoot->left = *(root);                   \
		(*(root))->right = tmp;                    \
                                                   \
		AVL_NODE_UPDATE_HEIGHT(newRoot->left);     \
		AVL_NODE_UPDATE_HEIGHT(newRoot);           \
                                                   \
		*(root) = newRoot;                         \
	} while (0)

#define AVL_NODE_RIGHT_ROTATE(root)                \
	do {                                           \
		avlNode *newRoot = (*(root))->left;        \
		avlNode *tmp = newRoot->right;             \
                                                   \
		newRoot->right = *(root);                  \
		(*(root))->left = tmp;                     \
                                                   \
		AVL_NODE_UPDATE_HEIGHT(newRoot->right);    \
		AVL_NODE_UPDATE_HEIGHT(newRoot);           \
                                                   \
		*(root) = newRoot;                         \
	} while (0)

#define AVL_NODE_LEFT_RIGHT_ROTATE(root)           \
	do {                                           \
		AVL_NODE_LEFT_ROTATE(&(*(root))->left);    \
		AVL_NODE_RIGHT_ROTATE((root));             \
	} while (0)

#define AVL_NODE_RIGHT_LEFT_ROTATE(root)           \
	do {                                           \
	AVL_NODE_RIGHT_ROTATE(&(*(root))->right);      \
	AVL_NODE_LEFT_ROTATE((root));                  \
	} while (0)

avlNode *avl_node_new(void *key, void *data)
{
	avlNode *n = malloc(sizeof(avlNode));
	n->key = key;
	n->data = data;
	n->height = 1;
	n->left = NULL;
	n->right = NULL;

	return n;
}

avlNode *avl_get_min(avlNode *root)
{
	avlNode *curr = root;

	while (curr->left != NULL) {
		curr = curr->left;
	}

	return curr;
}

avlNode *avl_find_node(avlNode *root, void *key, int cmpKeys(void *,void *))
{
	int cmp_res;

	while (root && (cmp_res = cmpKeys(key, root->key))) {
		if (cmp_res > 0) {
			root = root->right;
		} else {
			root = root->left;
		}
	}

	return root;
}

bool avl_is_exists(avlNode *root, void *key, int cmpKeys(void *,void *))
{
	return avl_find_node(root, key, cmpKeys) != NULL;
}

void *avl_find(avlNode *root, void *key, int cmpKeys(void *,void *))
{
	avlNode *node = avl_find_node(root, key, cmpKeys);

	if (node != NULL) {
		return node->data;
	}

	return NULL;
}

void avl_insert(avlNode **root, void *key, void *data, int cmpKeys(void *,void *))
{
	if (*root == NULL) {
		*root = avl_node_new(key, data);
		return;
	} else if (cmpKeys(key, (*root)->key) > 0) {
		avl_insert(&(*root)->right, key, data, cmpKeys);
	} else {
		avl_insert(&(*root)->left, key, data, cmpKeys);
	}

	AVL_NODE_UPDATE_HEIGHT(*root);

	int bf = AVL_NODE_GET_BALANCE_FACTOR(*root);

	if (bf > 1 && cmpKeys(key, (*root)->left->key) < 0) {
		AVL_NODE_RIGHT_ROTATE(root);
	} else if (bf < -1 && cmpKeys(key, (*root)->right->key) > 0) {
		AVL_NODE_LEFT_ROTATE(root);
	} else if (bf > 1 && cmpKeys(key, (*root)->left->key) > 0) {
		AVL_NODE_LEFT_RIGHT_ROTATE(root);
	} else if (bf < -1 && cmpKeys(key, (*root)->right->key) < 0) {
		AVL_NODE_RIGHT_LEFT_ROTATE(root);
	}
}

void avl_remove(avlNode **root, void *key, int cmpKeys(void *,void *), void freeKey(void *), void freeData(void *))
{
	if (*root == NULL) {
		return;
	} else if (cmpKeys(key, (*root)->key) > 0) {
		avl_remove(&((*root)->right), key, cmpKeys, freeKey, freeData);
		return;
	} else if (cmpKeys(key, (*root)->key) < 0) {
		avl_remove(&((*root)->left), key, cmpKeys, freeKey, freeData);
		return;
	} else {
		if (freeKey) {
			freeKey((*root)->key);
		}

		if (freeData) {
			freeData((*root)->data);
		}

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

		avlNode *succesor = avl_get_min((*root)->right);

		(*root)->key = succesor->key;
		(*root)->data = succesor->data;

		avl_remove(&((*root)->right), succesor->key, cmpKeys, NULL, NULL);
	}

	AVL_NODE_UPDATE_HEIGHT(*root);
	int bf = AVL_NODE_GET_BALANCE_FACTOR(*root);

	if (bf > 1 && AVL_NODE_GET_BALANCE_FACTOR((*root)->left) >= 0) {
		AVL_NODE_RIGHT_ROTATE(root);
	} else if (bf < -1 && AVL_NODE_GET_BALANCE_FACTOR((*root)->right) <= 0) {
		AVL_NODE_LEFT_ROTATE(root);
	} else if (bf > 1 && AVL_NODE_GET_BALANCE_FACTOR((*root)->left) < 0) {
		AVL_NODE_LEFT_RIGHT_ROTATE(root);
	} else if (bf < -1 && AVL_NODE_GET_BALANCE_FACTOR((*root)->right) > 0) {
		AVL_NODE_RIGHT_LEFT_ROTATE(root);
	}
}

void avl_update(avlNode *root, void *key, int cmpKeys(void *,void *), void freeData(void *), void *newData)
{
	avlNode *node = avl_find_node(root, key, cmpKeys);

	if (node == NULL) {
		return;
	}

	if (freeData) {
		freeData(node->data);
	}

	node->data = newData;
}

void avl_order_traverse(avlNode *root, void action(void *key, void *data, void *arg), void *arg)
{
	if (root == NULL) {
		return;
	}

    avl_order_traverse(root->left, action, arg);
    action(root->key, root->data, arg);
    avl_order_traverse(root->right, action, arg);
}

void printCharNTimes(char c, int n)
{
	for (int i = 0; i < n; i++) {
		putchar(c);
	}
}

void avl_print(avlNode *root, void print(void *key, void *data, void *arg), void *arg, int padding)
{
	if (root == NULL) {
		return;
	}

	printCharNTimes(' ', padding);
	print(root->key, root->data, arg);
	avl_print(root->right, print, arg, padding + 4);
	avl_print(root->left, print, arg, padding + 4);
}

void avl_free(avlNode *root, void freeKey(void *), void freeData(void *))
{
	if (root == NULL) {
		return;
	}

	if (freeKey) {
		freeKey(root->key);
	}

	if (freeData) {
		freeData(root->data);
	}

	avl_free(root->left, freeKey, freeData);
	avl_free(root->right, freeKey, freeData);

	free(root);
}
