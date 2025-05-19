#include "avl_tree.h"

#define CALL_F_IF_NOT_NULL(f, ...) if (f) f(__VA_ARGS__)

// #define Z_AVL_IMPLEMENT(type_name, K, V, prefix)

#define prefix avl
#define type_name AvlNode
#define K int
#define V int

int prefix##_get_height(const type_name *node)
{
	if (node == NULL) {
		return 0;
	}

	return node->height;
}

void prefix##_update_height(type_name *node)
{
	node->height = 1 + max(get_height(node->right), get_height(node->left));
}

int prefix##_get_balance_factor(const type_name *node)
{
	if (node == NULL) {
		return 0;
	}

	return get_height(node->left) - get_height(node->right);
}

void prefix##_left_rotate(type_name **root)
{
	type_name *newRoot = (*root)->right;
	type_name *tmp = newRoot->left;

	newRoot->left = *root;
	(*root)->right = tmp;

	update_height(newRoot->left);
	update_height(newRoot);

	*root = newRoot;
}

void prefix##_right_rotate(type_name **root)
{
	type_name *newRoot = (*root)->left;
	type_name *tmp = newRoot->right;

	newRoot->right = *root;
	(*root)->left = tmp;

	update_height(newRoot->right);
	update_height(newRoot);

	*root = newRoot;
}

void prefix##_left_right_rotate(type_name **root)
{
	left_rotate(&(*root)->left);
	right_rotate(root);
}

void prefix##_right_left_rotate(type_name **root)
{
	right_rotate(&(*root)->right);
	left_rotate(root);
}

type_name *prefix##_new(K key, V value)
{
	type_name *n = malloc(sizeof(type_name));
	n->key = key;
	n->data = value;
	n->height = 1;
	n->left = NULL;
	n->right = NULL;

	return n;
}

type_name *prefix##_get_min(type_name *root)
{
	type_name *curr = root;

	while (curr->left != NULL) {
		curr = curr->left;
	}

	return curr;
}

type_name *prefix##_find_node(type_name *root, K key, int cmp_keys(K, K))
{
	type_name *curr = root;

	while (curr != NULL) {
		int cmp_res = cmp_keys(key, curr->key);

		if (cmp_res > 0) {
			curr = curr->right;
		} else if (cmp_res < 0) {
			curr = curr->left;
		} else {
			return curr;
		}
	}

	return NULL;
}

bool prefix##_is_exists(type_name *root, K key, int cmp_keys(K, K))
{
	return avl_find_node(root, key, cmp_keys) != NULL;
}

V prefix##_avl_find(type_name *root, K key, int cmp_keys(K, K))
{
	type_name *node = avl_find_node(root, key, cmp_keys);

	if (node != NULL) {
		return node->value;
	}

	return NULL;
}

void prefix##_put(type_name **root, K key, V value, int cmp_keys(K, K), int free_value(V))
{
	if (*root == NULL) {
		*root = avl_node_new(key, value);
		return;
	}

	int cmp_res = cmp_keys(key, (*root)->key);

	if (cmp_res > 0) {
		avl_insert(&(*root)->right, key, value, cmp_keys);
	} else if (cmp_res < 0) {
		avl_insert(&(*root)->left, key, value, cmp_keys);
	} else {

		CALL_F_IF_NOT_NULL(free_value, (*root)->value);

		(*root)->value = value;
	}

	update_height(*root);

	int bf = get_balance_factor(*root);

	if (bf > 1 && cmp_keys(key, (*root)->left->key) < 0) {
		right_rotate(root);
	} else if (bf < -1 && cmp_keys(key, (*root)->right->key) > 0) {
		left_rotate(root);
	} else if (bf > 1 && cmp_keys(key, (*root)->left->key) > 0) {
		left_right_rotate(root);
	} else if (bf < -1 && cmp_keys(key, (*root)->right->key) < 0) {
		right_left_rotate(root);
	}
}

void prefix##_remove(type_name **root, K key, int cmp_keys(K, K), void free_key(K), void free_value(void *))
{
	if (*root == NULL) {
		return;
	}

	int cmp_res = cmp_keys(key, (*root)->key);

	if (cmp_res > 0) {
		avl_remove(&((*root)->right), key, cmp_keys, freeKey, free_value);
		return;
	}

	if (cmp_res < 0) {
		avl_remove(&((*root)->left), key, cmp_keys, freeKey, free_value);
		return;
	}

	// we reached the node

	CALL_F_IF_NOT_NULL(free_key, (*root)->key);
	CALL_F_IF_NOT_NULL(free_value, (*root)->value);

	if ((*root)->left == NULL) {
		type_name *tmp = (*root)->right;
		free(*root);
		*root = tmp;
		return;
	} else if ((*root)->right == NULL) {
		type_name *tmp = (*root)->left;
		free(*root);
		*root = tmp;
		return;
	}

	type_name *succesor = avl_get_min((*root)->right);

	(*root)->key = succesor->key;
	(*root)->data = succesor->data;

	avl_remove(&((*root)->right), succesor->key, cmp_keys, NULL, NULL);

	update_height(*root);
	int bf = get_balance_factor(*root);

	if (bf > 1 && get_balance_factor((*root)->left) >= 0) {
		right_rotate(root);
	} else if (bf < -1 && get_balace_factor((*root)->right) <= 0) {
		left_rotate(root);
	} else if (bf > 1 && get_balace_factor((*root)->left) < 0) {
		left_right_rotate(root);
	} else if (bf < -1 && get_balace_factor((*root)->right) > 0) {
		right_left_rotate(root);
	}
}

void prefix##_order_traverse(type_name *root, void action(K key, V value, void *arg), void *arg)
{
	if (root == NULL) {
		return;
	}

    avl_order_traverse(root->left, action, arg);
    action(root->key, root->data, arg);
    avl_order_traverse(root->right, action, arg);
}

void print_char_n_times(char c, int n)
{
	for (int i = 0; i < n; i++) {
		putchar(c);
	}
}

void prefix##_print(type_name *root, void print(K key, V value, void *arg), void *arg, int padding)
{
	if (root == NULL) {
		return;
	}

	print_char_n_times(' ', padding);
	print(root->key, root->data, arg);
	avl_print(root->right, print, arg, padding + 4);
	avl_print(root->left, print, arg, padding + 4);
}

void prefix##_free(type_name *root, void free_key(K), void free_value(V))
{
	if (root == NULL) {
		return;
	}

	CALL_F_IF_NOT_NULL(free_key, root->key);
	CALL_F_IF_NOT_NULL(free_value, root->value);

	avl_free(root->left, free_key, free_value);
	avl_free(root->right, free_key, free_value);

	free(root);
}
