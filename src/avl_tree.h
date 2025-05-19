#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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

// TODO: insert to put

#define Z_AVL_DECLARE(type_name, key_t, value_t, prefix)                            \
                                                                                    \
typedef struct type_name {                                                          \
	struct type_name *left;                                                         \
	struct type_name *right;                                                        \
	key_t key;                                                                      \
	value_t value;                                                                  \
	int height;                                                                     \
} type_name;                                                                        \
                                                                                    \
void prefix##_insert(type_name **root,                                              \
				     key_t key,                                                     \
				     value_t value,                                                 \
				     int cmp_keys(key_t, key_t));                                   \
                                                                                    \
bool prefix##_is_exists(type_name *root,                                            \
						key_t key,                                                  \
						int cmp_keys(key_t, key_t));                                \
                                                                                    \
value_t prefix##_find(type_name *root,                                              \
					  key_t key,                                                    \
					  int cmp_keys(key_t, key_t));                                  \
                                                                                    \
void prefix##_remove(type_name **root,                                              \
					 key_t key,                                                     \
					 int cmp_keys(key_t, key_t),                                    \
					 void free_key(key_t),                                          \
					 void free_value(value_t));                                     \
                                                                                    \
void prefix##_update(type_name *root,                                               \
					 key_t key,                                                     \
					 int cmp_keys(key_t, key_t),                                    \
					 void free_value(value_t),                                      \
					 void *newData);                                                \
                                                                                    \
void prefix##_order_traverse(type_name *root,                                       \
		                     void action(key_t key, value_t value, void *arg),      \
							 void *arg);                                            \
                                                                                    \
void prefix##_print(type_name *root,                                                \
				    void print(key_t key, value_t value, void *arg),                \
					void *arg, int padding);                                        \
                                                                                    \
void prefix##_free(type_name *root,                                                 \
		           void free_key(key_t),                                            \
				   void free_value(value_t));

#endif
