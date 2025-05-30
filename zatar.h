/*
 *           --Libzatar--
 *
 *  #define LIBZATAR_IMPLEMENTATION
 */
#ifndef LIBZATAR_H
#define LIBZATAR_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   util header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

#define Z_DEFAULT_GROWTH_RATE 2

#define CALL_F_IF_NOT_NULL(f, ...) if (f) f(__VA_ARGS__)

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

typedef enum { Z_Ok, Z_Err } Z_Result;

int z_in_range(int min, int val, int max);
int z_get_file_size(FILE *fp);
int z_get_fmt_size(const char *fmt, ...);
int z_get_fmt_size_va(const char *fmt, va_list ap);
void *z_memdup(const void *mem, const size_t size);
void z_swap(void *a, void *b, const size_t size);
int z_max(int a, int b);
int z_min(int a, int b);
int z_max3(int a, int b, int c);
int z_min3(int a, int b, int c);

int z_print_error(const char *fmt, ...);
int z_print_info(const char *fmt, ...);
int z_print_warning(const char *fmt, ...);

#define z_ensure_capacity(da, cap)                                                     \
    do {                                                                               \
        if ((da)->capacity < (cap)) {                                                      \
            int new_capacity = z_max((cap), (da)->capacity * Z_DEFAULT_GROWTH_RATE);   \
            (da)->capacity = new_capacity;                                             \
            (da)->ptr = realloc((da)->ptr, sizeof((da)->ptr[0]) * (da)->capacity);     \
        }                                                                              \
    } while (0)

#define z_null_terminate(da)                                   \
    do {                                                       \
        z_ensure_capacity((da), (da)->len + 1);                \
        memset(&(da)->ptr[(da)->len], 0, sizeof(*(da)->ptr));  \
    } while(0)

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   cursor header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

#define CTRL_KEY(k)             ((k) & 0x1f)

#define Z_COLOR_RESET           "\033[0m"

#define Z_COLOR_RED             "\033[0;31m"
#define Z_COLOR_GREEN           "\033[0;32m"
#define Z_COLOR_YELLOW          "\033[0;33m"
#define Z_COLOR_BLUE            "\033[0;34m"
#define Z_COLOR_MAGENTA         "\033[0;35m"
#define Z_COLOR_CYAN            "\033[0;36m"
#define Z_COLOR_WHITE            "\033[0;37m"
#define Z_COLOR_GRAY            "\033[0;90m"

#define Z_COLOR_BOLD_RED        "\033[1;91m"
#define Z_COLOR_BOLD_GREEN      "\033[1;92m"
#define Z_COLOR_BOLD_YELLOW     "\033[1;93m"
#define Z_COLOR_BOLD_BLUE       "\033[1;94m"
#define Z_COLOR_BOLD_MAGENTA    "\033[1;95m"
#define Z_COLOR_BOLD_CYAN       "\033[1;96m"
#define Z_COLOR_BOLD_WHITE      "\033[1;97m"
#define Z_COLOR_BOLD_GRAY       "\033[1;90m"

#define Z_KEY_EMPTY         999
#define Z_KEY_ARROW_LEFT    1000
#define Z_KEY_ARROW_RIGHT   1001
#define Z_KEY_ARROW_UP      1002
#define Z_KEY_ARROW_DOWN    1003
#define Z_KEY_PAGE_UP       1007
#define Z_KEY_PAGE_DOWN     1008
#define Z_KEY_DELETE        1004
#define Z_KEY_HOME          1005
#define Z_KEY_END           1006

typedef enum {
    Z_CURSOR_STYLE_BLOCK_STEADY       = 0,
    Z_CURSOR_STYLE_BLOCK_BLINKING     = 1,
    Z_CURSOR_STYLE_UNDERLINE_BLINKING = 3,
    Z_CURSOR_STYLE_UNDERLINE_STEADY   = 4,
    Z_CURSOR_STYLE_BEAM_STEADY        = 6,
    Z_CURSOR_STYLE_BEAM_BLINKING      = 5,
} Z_CURSOR_STYLE;

#define z_set_cursor_style(style)         printf("\033[%dq", (int)(style))
#define z_disable_line_wrap()             printf("\033[?7l")
#define z_enbale_line_wrap()              printf("\033[?7h")
#define z_hide_cursor()                   printf("\033[?25l")
#define z_show_cursor()                   printf("\033[?25h")
#define z_set_cursor_pos(x, y)            printf("\033[%d;%dH", (y), (x))
#define z_set_cursor_x(x)                 printf("\033[%dG", (x))
#define z_cursor_up(n)                    printf("\033[%dA", (n))
#define z_cursor_down(n)                  printf("\033[%dB", (n))
#define z_cursor_right(n)                 printf("\033[%dC", (n))
#define z_cursor_left(n)                  printf("\033[%dD", (n))
// unsupported on some terminals
#define z_save_cursor_pos()               printf("\033[s")
#define z_restore_cursor_pos()            printf("\033[u")
#define z_enter_alternative_screen()      printf("\033[?1049h")
#define z_exit_alternative_screen()       printf("\033[?1049l")
#define z_clear_line()                    printf("\033[K")
#define z_clear_screen()                  printf("\033[2J")
#define z_update_screen()                 fflush(stdout)

Z_Result z_enable_raw_mode(int vminKeys, int vtime);
Z_Result z_disable_raw_mode();

Z_Result z_get_cursor_pos(int *x, int *y);

Z_Result z_get_screen_size_by_cursor(int *width, int *height);
Z_Result z_get_screen_size_by_ioctl(int *width, int *height);
Z_Result z_get_screen_size(int *width, int *height);

Z_Result z_register_change_in_window_size(void function(int));

Z_Result z_enable_full_buffering(FILE *fp);

int z_wait_for_byte();
int z_read_escape_key();
int z_read_key();

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   vector header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

#define Z_VECTOR_DECLARE(name, T, prefix)                               \
                                                                       \
typedef struct {                                                       \
    T *ptr;                                                            \
    int len;                                                           \
    int capacity;                                                      \
} name;                                                                \
                                                                       \
void prefix##_init(name *v);                                           \
T prefix##_at(name *v, int i);                                         \
void prefix##_add(name *v, T data);                                    \
T prefix##_remove_last(name *v);                                       \
int prefix##_len(const name *v);                                       \
bool prefix##_is_empty(const name *v);                                 \
int prefix##_find(const name *v, T data, void cmp(T, T));              \
void prefix##_for_each(name *v, void function(T));                     \
void prefix##_free(name *v, void free_function(T));                    \
void prefix##_print(const name *v, void print_data(T));


#define Z_VECTOR_IMPLEMENT(name, T, prefix)                            \
                                                                       \
void prefix##_init(name *v)                                            \
{                                                                      \
    v->ptr = NULL;                                                     \
    v->len = 0;                                                        \
    v->capacity = 0;                                                   \
}                                                                      \
                                                                       \
T prefix##_at(name *v, int i)                                          \
{                                                                      \
    return v->ptr[i];                                                  \
}                                                                      \
                                                                       \
void prefix##_add(name *v, T data)                                     \
{                                                                      \
    z_ensure_capacity(v, v->len + 1);                                  \
    v->ptr[v->len++] = data;                                           \
}                                                                      \
                                                                       \
T prefix##_remove_last(name *v)                                        \
{                                                                      \
    return v->ptr[--v->len];                                           \
}                                                                      \
                                                                       \
int prefix##_len(const name *v)                                        \
{                                                                      \
    return v->len;                                                     \
}                                                                      \
                                                                       \
bool prefix##_is_empty(const name *v)                                  \
{                                                                      \
    return v->len == 0;                                                \
}                                                                      \
                                                                       \
int prefix##_find(const name *v, T data, void cmp(T, T))               \
{                                                                      \
    if (cmp) {                                                         \
        for (int i = 0; i < v->len; i++) {                             \
            if (cmp(data, v->ptr[i]) == 0) {                           \
                return i;                                              \
            }                                                          \
        }                                                              \
    } else {                                                           \
        for (int i = 0; i < v->len; i++) {                             \
            if (data == v->ptr[i]) {                                   \
                return i;                                              \
            }                                                          \
        }                                                              \
    }                                                                  \
                                                                       \
    return -1;                                                         \
}                                                                      \
                                                                       \
void prefix##_for_each(name *v, void function(T))                      \
{                                                                      \
    for (int i = 0; i < v->len; i++) {                                 \
        function(v->ptr[i]);                                           \
    }                                                                  \
}                                                                      \
                                                                       \
void prefix##_free(name *v, void free_function(T))                     \
{                                                                      \
    if (free_function) {                                               \
        prefix##_for_each(v, free_function);                           \
    } else {                                                           \
        free(v->ptr);                                                  \
    }                                                                  \
}                                                                      \
                                                                       \
void prefix##_print(const name *v, void print_data(T))                 \
{                                                                      \
    if (v->len == 0) {                                                 \
        printf("[]\n");                                                \
        return;                                                        \
    }                                                                  \
                                                                       \
    printf("[ ");                                                      \
                                                                       \
    for (int i = 0; i < v->len - 1; i++) {                             \
        print_data(v->ptr[i]);                                         \
        printf(", ");                                                  \
    }                                                                  \
                                                                       \
    print_data(v->ptr[v->len - 1]);                                    \
                                                                       \
    printf(" ]\n");                                                    \
}

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   avl tree header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

#define Z_AVL_DECLARE(type_name, K, V, prefix)                         \
                                                                       \
typedef struct type_name {                                             \
    struct type_name *left;                                            \
    struct type_name *right;                                           \
    K key;                                                             \
    V value;                                                           \
    char height;                                                       \
} type_name;                                                           \
                                                                       \
void prefix##_put(type_name **root,                                    \
                     K key,                                            \
                     V value,                                          \
                     int cmp_keys(K, K),                               \
                     void free_key(K),                                 \
                     void free_value(V));                              \
                                                                       \
bool prefix##_is_exists(type_name *root,                               \
                        K key,                                         \
                        int cmp_keys(K, K));                           \
                                                                       \
bool prefix##_find(type_name *root,                                    \
                K key,                                                 \
                int cmp_keys(K, K),                                    \
                V *value);                                             \
                                                                       \
void prefix##_remove(type_name **root,                                 \
                     K key,                                            \
                     int cmp_keys(K, K),                               \
                     void free_key(K),                                 \
                     void free_value(V));                              \
                                                                       \
void prefix##_order_traverse(type_name *root,                          \
                             void action(K key, V value, void *arg),   \
                             void *arg);                               \
                                                                       \
void prefix##_print(type_name *root,                                   \
                    void print(K key, V value, void *arg),             \
                    void *arg, int padding);                           \
                                                                       \
void prefix##_free(type_name *root,                                    \
                   void free_key(K),                                   \
                   void free_value(V));



#define Z_AVL_IMPLEMENT(type_name, K, V, prefix)                              \
                                                                              \
int prefix##_get_height(const type_name *node)                                \
{                                                                             \
    if (node == NULL) {                                                       \
        return 0;                                                             \
    }                                                                         \
                                                                              \
    return node->height;                                                      \
}                                                                             \
                                                                              \
void prefix##_update_height(type_name *node)                                  \
{                                                                             \
    node->height = 1 + z_max(                                                 \
            prefix##_get_height(node->right),                                 \
            prefix##_get_height(node->left)                                   \
    );                                                                        \
}                                                                             \
                                                                              \
int prefix##_get_balance_factor(const type_name *node)                        \
{                                                                             \
    if (node == NULL) {                                                       \
        return 0;                                                             \
    }                                                                         \
                                                                              \
    return prefix##_get_height(node->left) - prefix##_get_height(node->right);\
}                                                                             \
                                                                              \
void prefix##_left_rotate(type_name **root)                                   \
{                                                                             \
    type_name *newRoot = (*root)->right;                                      \
    type_name *tmp = newRoot->left;                                           \
                                                                              \
    newRoot->left = *root;                                                    \
    (*root)->right = tmp;                                                     \
                                                                              \
    prefix##_update_height(newRoot->left);                                    \
    prefix##_update_height(newRoot);                                          \
                                                                              \
    *root = newRoot;                                                          \
}                                                                             \
                                                                              \
void prefix##_right_rotate(type_name **root)                                  \
{                                                                             \
    type_name *newRoot = (*root)->left;                                       \
    type_name *tmp = newRoot->right;                                          \
                                                                              \
    newRoot->right = *root;                                                   \
    (*root)->left = tmp;                                                      \
                                                                              \
    prefix##_update_height(newRoot->right);                                   \
    prefix##_update_height(newRoot);                                          \
                                                                              \
    *root = newRoot;                                                          \
}                                                                             \
                                                                              \
void prefix##_left_right_rotate(type_name **root)                             \
{                                                                             \
    prefix##_left_rotate(&(*root)->left);                                     \
    prefix##_right_rotate(root);                                              \
}                                                                             \
                                                                              \
void prefix##_right_left_rotate(type_name **root)                             \
{                                                                             \
    prefix##_right_rotate(&(*root)->right);                                   \
    prefix##_left_rotate(root);                                               \
}                                                                             \
                                                                              \
type_name *prefix##_new(K key, V value)                                       \
{                                                                             \
    type_name *n = malloc(sizeof(type_name));                                 \
    n->key = key;                                                             \
    n->value = value;                                                         \
    n->height = 1;                                                            \
    n->left = NULL;                                                           \
    n->right = NULL;                                                          \
                                                                              \
    return n;                                                                 \
}                                                                             \
                                                                              \
type_name *prefix##_get_min(type_name *root)                                  \
{                                                                             \
    type_name *curr = root;                                                   \
                                                                              \
    while (curr->left != NULL) {                                              \
        curr = curr->left;                                                    \
    }                                                                         \
                                                                              \
    return curr;                                                              \
}                                                                             \
                                                                              \
type_name *prefix##_find_node(type_name *root, K key, int cmp_keys(K, K))     \
{                                                                             \
    type_name *curr = root;                                                   \
                                                                              \
    while (curr != NULL) {                                                    \
        int cmp_res = cmp_keys(key, curr->key);                               \
                                                                              \
        if (cmp_res > 0) {                                                    \
            curr = curr->right;                                               \
        } else if (cmp_res < 0) {                                             \
            curr = curr->left;                                                \
        } else {                                                              \
            return curr;                                                      \
        }                                                                     \
    }                                                                         \
                                                                              \
    return NULL;                                                              \
}                                                                             \
                                                                              \
bool prefix##_is_exists(type_name *root,                                      \
                        K key,                                                \
                        int cmp_keys(K, K))                                   \
{                                                                             \
    return prefix##_find_node(root, key, cmp_keys) != NULL;                        \
}                                                                             \
                                                                              \
bool prefix##_find(type_name *root,                                           \
                K key,                                                        \
                int cmp_keys(K, K),                                           \
                V *value)                                                     \
{                                                                             \
    type_name *node = prefix##_find_node(root, key, cmp_keys);                     \
                                                                              \
    if (node != NULL) {                                                       \
        *value = node->value;                                                 \
        return true;                                                          \
    }                                                                         \
                                                                              \
    return false;                                                             \
}                                                                             \
                                                                              \
void prefix##_put(type_name **root,                                           \
                     K key,                                                   \
                     V value,                                                 \
                     int cmp_keys(K, K),                                      \
                     void free_key(K),                                        \
                     void free_value(V))                                      \
{                                                                             \
    if (*root == NULL) {                                                      \
        *root = prefix##_new(key, value);                                     \
        return;                                                               \
    }                                                                         \
                                                                              \
    int cmp_res = cmp_keys(key, (*root)->key);                                \
                                                                              \
    if (cmp_res > 0) {                                                        \
        prefix##_put(&(*root)->right, key, value, cmp_keys,                   \
                free_key, free_value);                                        \
    } else if (cmp_res < 0) {                                                 \
        prefix##_put(&(*root)->left, key, value, cmp_keys,                    \
                free_key, free_value);                                        \
    } else {                                                                  \
        CALL_F_IF_NOT_NULL(free_value, (*root)->value);                       \
        CALL_F_IF_NOT_NULL(free_key, key);                                  \
        (*root)->value = value;                                               \
    }                                                                         \
                                                                              \
    prefix##_update_height(*root);                                            \
                                                                              \
    int bf = prefix##_get_balance_factor(*root);                              \
                                                                              \
    if (bf > 1 && cmp_keys(key, (*root)->left->key) < 0) {                    \
        prefix##_right_rotate(root);                                          \
    } else if (bf < -1 && cmp_keys(key, (*root)->right->key) > 0) {           \
        prefix##_left_rotate(root);                                           \
    } else if (bf > 1 && cmp_keys(key, (*root)->left->key) > 0) {             \
        prefix##_left_right_rotate(root);                                     \
    } else if (bf < -1 && cmp_keys(key, (*root)->right->key) < 0) {           \
        prefix##_right_left_rotate(root);                                     \
    }                                                                         \
}                                                                             \
                                                                              \
void prefix##_remove(type_name **root,                                        \
                     K key,                                                   \
                     int cmp_keys(K, K),                                      \
                     void free_key(K),                                        \
                     void free_value(V))                                      \
{                                                                             \
    if (*root == NULL) {                                                      \
        return;                                                               \
    }                                                                         \
                                                                              \
    int cmp_res = cmp_keys(key, (*root)->key);                                \
                                                                              \
    if (cmp_res > 0) {                                                        \
        prefix##_remove(&((*root)->right), key, cmp_keys, free_key, free_value);   \
        return;                                                               \
    }                                                                         \
                                                                              \
    if (cmp_res < 0) {                                                        \
        prefix##_remove(&((*root)->left), key, cmp_keys, free_key, free_value);    \
        return;                                                               \
    }                                                                         \
                                                                              \
    /* we reached the node */                                                 \
                                                                              \
    CALL_F_IF_NOT_NULL(free_key, (*root)->key);                               \
    CALL_F_IF_NOT_NULL(free_value, (*root)->value);                           \
                                                                              \
    if ((*root)->left == NULL) {                                              \
        type_name *tmp = (*root)->right;                                      \
        free(*root);                                                          \
        *root = tmp;                                                          \
        return;                                                               \
    } else if ((*root)->right == NULL) {                                      \
        type_name *tmp = (*root)->left;                                       \
        free(*root);                                                          \
        *root = tmp;                                                          \
        return;                                                               \
    }                                                                         \
                                                                              \
    type_name *succesor = prefix##_get_min((*root)->right);                   \
                                                                              \
    (*root)->key = succesor->key;                                             \
    (*root)->value = succesor->value;                                         \
                                                                              \
    prefix##_remove(&((*root)->right), succesor->key, cmp_keys, NULL, NULL);  \
                                                                              \
    prefix##_update_height(*root);                                            \
    int bf = prefix##_get_balance_factor(*root);                              \
                                                                              \
    if (bf > 1 && prefix##_get_balance_factor((*root)->left) >= 0) {          \
        prefix##_right_rotate(root);                                          \
    } else if (bf < -1 && prefix##_get_balance_factor((*root)->right) <= 0) { \
        prefix##_left_rotate(root);                                           \
    } else if (bf > 1 && prefix##_get_balance_factor((*root)->left) < 0) {    \
        prefix##_left_right_rotate(root);                                     \
    } else if (bf < -1 && prefix##_get_balance_factor((*root)->right) > 0) {  \
        prefix##_right_left_rotate(root);                                     \
    }                                                                         \
}                                                                             \
                                                                              \
void prefix##_order_traverse(type_name *root,                                 \
                             void action(K key, V value, void *arg),          \
                             void *arg)                                       \
{                                                                             \
    if (root == NULL) {                                                       \
        return;                                                               \
    }                                                                         \
                                                                              \
    prefix##_order_traverse(root->left, action, arg);                              \
                                                                              \
    action(root->key, root->value, arg);                                      \
    prefix##_order_traverse(root->right, action, arg);                             \
}                                                                             \
                                                                              \
void print_char_n_times(char c, int n)                                        \
{                                                                             \
    for (int i = 0; i < n; i++) {                                             \
        putchar(c);                                                           \
    }                                                                         \
}                                                                             \
                                                                              \
void prefix##_print(type_name *root,                                          \
                    void print(K key, V value, void *arg),                    \
                    void *arg, int padding)                                   \
{                                                                             \
    if (root == NULL) {                                                       \
        return;                                                               \
    }                                                                         \
                                                                              \
    print_char_n_times(' ', padding);                                         \
    print(root->key, root->value, arg);                                       \
    prefix##_print(root->right, print, arg, padding + 4);                          \
    prefix##_print(root->left, print, arg, padding + 4);                           \
}                                                                             \
                                                                              \
void prefix##_free(type_name *root,                                           \
                   void free_key(K),                                          \
                   void free_value(V))                                        \
{                                                                             \
    if (root == NULL) {                                                       \
        return;                                                               \
    }                                                                         \
                                                                              \
    CALL_F_IF_NOT_NULL(free_key, root->key);                                  \
    CALL_F_IF_NOT_NULL(free_value, root->value);                              \
                                                                              \
    prefix##_free(root->left, free_key, free_value);                               \
    prefix##_free(root->right, free_key, free_value);                              \
                                                                              \
    free(root);                                                               \
}

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   map header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *


#define Z_MAP_DECLARE(type_name, K, V, prefix)                         \
                                                                       \
Z_AVL_DECLARE(_avl_##type_name, K, V, _avl_##prefix)                   \
                                                                       \
typedef struct {                                                       \
    _avl_##type_name *root;                                            \
    int (*cmp_keys)(K, K);                                             \
} type_name;                                                           \
                                                                       \
void prefix##_init(type_name *m, int cmp_keys(K, K));                  \
                                                                       \
void prefix##_put(type_name *m,                                        \
                  K key,                                               \
                  V value,                                             \
                  void free_key(K),                                    \
                  void free_value(V));                                 \
                                                                       \
bool prefix##_find(const type_name *m, K key, V *value);               \
                                                                       \
bool prefix##_is_exists(const type_name *m, K key);                    \
                                                                       \
void prefix##_remove(type_name *m, K key,                              \
                     void free_key(K),                                 \
                     void free_value(V));                              \
                                                                       \
void prefix##_order_traverse(const type_name *m,                       \
                             void action(K key, V value, void *arg),   \
                             void *arg);                               \
                                                                       \
void prefix##_free(type_name *m,                                       \
                   void free_key(K),                                   \
                   void free_value(V));

#define Z_MAP_IMPLEMENT(type_name, K, V, prefix)                       \
                                                                       \
Z_AVL_IMPLEMENT(_avl_##type_name, K, V, _avl_##prefix)                 \
                                                                       \
void prefix##_init(type_name *m, int cmp_keys(K, K))                   \
{                                                                      \
    m->cmp_keys = cmp_keys;                                            \
    m->root = NULL;                                                    \
}                                                                      \
                                                                       \
void prefix##_put(type_name *m,                                        \
                  K key,                                               \
                  V value,                                             \
                  void free_key(K),                                    \
                  void free_value(V))                                  \
{                                                                      \
    _avl_##prefix##_put(&m->root, key, value, m->cmp_keys, free_key, free_value);\
}                                                                      \
                                                                       \
bool prefix##_find(const type_name *m, K key, V *value)                \
{                                                                      \
    return _avl_##prefix##_find(m->root, key, m->cmp_keys, value);     \
}                                                                      \
                                                                       \
bool prefix##_is_exists(const type_name *m, K key)                     \
{                                                                      \
    return _avl_##prefix##_is_exists(m->root, key, m->cmp_keys);       \
}                                                                      \
                                                                       \
void prefix##_remove(type_name *m, K key,                              \
                     void free_key(K),                                 \
                     void free_value(V))                               \
{                                                                      \
    _avl_##prefix##_remove(&m->root,                                   \
            key,                                                       \
            m->cmp_keys,                                               \
            free_key,                                                  \
            free_value);                                               \
}                                                                      \
                                                                       \
void prefix##_order_traverse(const type_name *m,                       \
                             void action(K key, V value, void *arg),   \
                             void *arg)                                \
{                                                                      \
    _avl_##prefix##_order_traverse(m->root, action, arg);              \
}                                                                      \
                                                                       \
void prefix##_free(type_name *m,                                       \
                   void free_key(K),                                   \
                   void free_value(V))                                 \
{                                                                      \
    _avl_##prefix##_free(m->root, free_key, free_value);               \
}


//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   matrix header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

#define Z_MAT_DECLARE(name, T)    \
typedef struct {                  \
    T *ptr;                       \
    int x;                        \
    int y;                        \
    int capacity;                 \
} name;

#define Z_MAT_AT(mat, _y, _x) ((mat)->ptr[(_y) * (mat)->x + (_x)])

#define Z_MAT_INIT(mat, _x, _y)                                     \
    do {                                                            \
        (mat)->ptr = malloc(sizeof((mat)->ptr[0]) * (_x) * (_y));   \
        (mat)->x = (_x);                                            \
        (mat)->y = (_y);                                            \
        (mat)->capacity = (_x) * (_y);                              \
    } while (0)

#define Z_MAT_RESIZE(mat, _x, _y)                                                   \
    do {                                                                            \
        if ((mat)->capacity < (_x) * (_y)) {                                        \
            (mat)->capacity = (_x) * (_y);                                          \
            (mat)->ptr = realloc((mat)->ptr, sizeof((mat)->ptr[0]) * (_x) * (_y));  \
        }                                                                           \
                                                                                    \
        (mat)->x = (_x);                                                            \
        (mat)->y = (_y);                                                            \
    } while (0)

#define Z_MAT_FREE(mat) do { free((mat)->ptr); (mat)->ptr = NULL; } while (0)

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   string header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

typedef struct {
    char *ptr;
    int len;
    int capacity;
} Z_String;

typedef struct {
    const char *ptr;
    int len;
} Z_String_View;

#define Z_SV(p, l)          ((Z_String_View){ .ptr = p, .len = l })
#define Z_STR_TO_SV(s)      ((Z_String_View){ .ptr = s.ptr, .len = s.len })
#define Z_CSTR_TO_SV(s)     ((Z_String_View){ .ptr = s, .len = strlen(s) })
#define Z_EMPTY_SV()        ((Z_String_View){ .ptr = NULL, .len = 0 })

Z_String z_str_new(const char *fmt, ...);
Z_String z_str_new_va(const char *fmt, va_list ap);
void z_str_pushf(Z_String *s, const char *fmt, ...);
void z_str_pushf_va(Z_String *s, const char *fmt, va_list ap);
void z_str_push(Z_String *dst, Z_String_View src);
void z_str_push_c(Z_String *s, char c);
char z_str_pop_c(Z_String *s);
char z_str_top_c(Z_String_View s);
int z_str_cmp(Z_String_View s1, Z_String_View s2);
int z_str_n_cmp(Z_String_View s1, Z_String_View s2, int n);
void z_str_replace(Z_String *s, Z_String_View target, Z_String_View replacement);

void z_str_trim(Z_String *s);
void z_str_trim_cset(Z_String *s, Z_String_View cset);
Z_String_View z_str_view_trim(Z_String_View s);
Z_String_View z_str_view_trim_cset(Z_String_View s, Z_String_View cset);

void z_str_print(Z_String_View s);
void z_str_println(Z_String_View s);
void z_str_free(Z_String *s);
void z_str_clear(Z_String *s);

Z_String z_read_whole_file(const char *pathname, Z_Result *result);

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   path header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

typedef enum {
    Z_Pipe_Mode_Read = 0,
    Z_Pipe_Mode_Write = 1,
} Z_Pipe_Mode;

Z_String_View z_get_path_extention(Z_String_View path);
Z_String_View z_get_path_basename(Z_String_View path);

Z_String_View z_get_home_path();

Z_String z_expand_path(Z_String_View s);
Z_String z_compress_path(Z_String_View s);

bool z_is_extention_equal(Z_String_View pathname, Z_String_View extention);

Z_Result z_dir_traverse(const char *dir, bool action(const char *));

bool z_is_dir(const char *pathname);
bool z_is_regular_file(const char *pathname);
bool z_is_path_exists(const char *pathname);

Z_Result z_write_file(const char *pathname, const char *fmt, ...);
Z_Result z_append_file(const char *pathname, const char *fmt, ...);
Z_Result z_read_file(const char *pathname, const char *fmt, ...);

Z_Result z_redirect_fd(int src_fd, const char *dst_pathname);
Z_Result z_popen2(char *path, char *argv[], FILE *ppipe[2]);

bool z_mkdir(const char *pathname);


//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   cmd header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

typedef struct {
	char **ptr;
	int len;
    int capacity;
} Z_Cmd;

bool _z_should_rebuild(const char *target, ...);
bool z_should_rebuild_va(const char *target, va_list ap);
#define z_should_rebuild(target, ...) _z_should_rebuild(target, ##__VA_ARGS__, NULL)
void z_rebuild_yourself(const char *src_pathname, char **argv);
void z_cmd_init(Z_Cmd *cmd);
#define z_cmd_append(cmd, ...) _z_cmd_append(cmd, __VA_ARGS__, NULL)
void _z_cmd_append(Z_Cmd *cmd, ...);
void z_cmd_append_va(Z_Cmd *cmd, va_list ap);
int z_cmd_run_async(Z_Cmd *cmd);
int _z_run_async(const char *arg, ...);
#define z_run_async(arg, ...) _z_run_async(arg, ##__VA_ARGS__, NULL)
void z_cmd_free(Z_Cmd *cmd);
void z_cmd_clear(Z_Cmd *cmd);

//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $


#ifdef LIBZATAR_IMPLEMENTATION


//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $




//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   util implementation
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

int z_print_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    printf("[" Z_COLOR_RED "ERROR" Z_COLOR_RESET "] ");
    int n = vprintf(fmt, ap);
    printf("\n");
    va_end(ap);

    return n;
}

int z_print_warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    printf("[" Z_COLOR_YELLOW "WARNING" Z_COLOR_RESET "] ");
    int n = vprintf(fmt, ap);
    printf("\n");
    va_end(ap);

    return n;
}

int z_print_info(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    printf("[" Z_COLOR_GREEN "INFO" Z_COLOR_RESET "] ");
    int n = vprintf(fmt, ap);
    printf("\n");
    va_end(ap);

    return n;
}

void z_swap(void *a, void *b, const size_t size)
{
    char tmp[size];
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
}

int z_get_file_size(FILE *fp)
{
    int curr = ftell(fp);
    fseek(fp, 0, SEEK_END);

    int size = ftell(fp);
    fseek(fp, curr, SEEK_SET);

    return size;
}

int z_get_fmt_size(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    int size = z_get_fmt_size_va(fmt, ap);
    va_end(ap);

    return size;
}

int z_get_fmt_size_va(const char *fmt, va_list ap)
{
    va_list ap1;
    va_copy(ap1, ap);

    int size = vsnprintf(NULL, 0, fmt, ap1);
    va_end(ap1);

    return size;
}

void *z_memdup(const void *mem, const size_t size)
{
    void *newMem = malloc(size);
    memcpy(newMem, mem, size);
    return newMem;
}

int z_max(int a, int b)
{
    return a > b ? a : b;
}

int z_min(int a, int b)
{
    return a > b ? b : a;
}

int z_min3(int a, int b, int c)
{
    return z_min(a, z_min(b, c));
}

int z_max3(int a, int b, int c)
{
    return z_max(a, z_max(b, c));
}

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   cursor implementation
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

static struct termios original_termios;

Z_Result z_enable_raw_mode(int vminKeys, int vtime)
{
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        return Z_Err;
    }

    struct termios raw = original_termios;
    raw.c_cc[VMIN] = vminKeys;
    raw.c_cc[VTIME] = vtime;
    raw.c_cflag |= (CS8);
    raw.c_oflag &= ~(OPOST);
    raw.c_iflag &= ~(IXON | ICRNL | ISTRIP | INPCK | BRKINT);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        return Z_Err;
    }

    return Z_Ok;
}

Z_Result z_disable_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1) {
        return Z_Err;
    }

    return Z_Ok;
}

Z_Result z_get_cursor_pos(int *x, int *y)
{
    printf("\033[6n");

    if (scanf("\033[%d;%dR", y, x) == 2) {
        return Z_Ok;
    }

    return Z_Err;
}

Z_Result z_get_screen_size_by_cursor(int *width, int *height)
{
    z_set_cursor_pos(999, 999);

    return z_get_cursor_pos(width, height);
}

Z_Result z_get_screen_size_by_ioctl(int *width, int *height)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0) {
        return Z_Err;
    }

    *width = ws.ws_col;
    *height = ws.ws_row;

    return Z_Ok;
}

Z_Result z_get_screen_size(int *width, int *height)
{
    if (z_get_screen_size_by_ioctl(width, height) == Z_Ok) {
        return Z_Ok;
    }

    return z_get_screen_size_by_cursor(width, height);
}

Z_Result z_register_change_in_window_size(void funciton(int))
{
    struct sigaction sa;
    sa.sa_handler = funciton;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGWINCH, &sa, NULL) == -1) {
        return Z_Err;
    }

    return Z_Ok;
}

Z_Result z_enable_full_buffering(FILE *fp)
{
    // do not flush on '\n'
    if (setvbuf(fp, NULL, _IOFBF, BUFSIZ) != 0) {
        return Z_Err;
    }

    return Z_Ok;
}

int z_wait_for_byte()
{
    char c;

    if (read(STDIN_FILENO, &c, 1) != 1) {
        return Z_KEY_EMPTY;
    }

    return c;
}

#define CHAR2_TO_INT(a, b) ((unsigned short)((((unsigned short)b) << 8) | a))

int z_read_escape_key()
{
    unsigned short key;

    if (read(STDIN_FILENO, &key, 2) != 2) {
        return '\033';
    }

    switch (key) {
        case CHAR2_TO_INT('[', 'A'): return Z_KEY_ARROW_UP;
        case CHAR2_TO_INT('[', 'B'): return Z_KEY_ARROW_DOWN;
        case CHAR2_TO_INT('[', 'C'): return Z_KEY_ARROW_RIGHT;
        case CHAR2_TO_INT('[', 'D'): return Z_KEY_ARROW_LEFT;
        case CHAR2_TO_INT('[', '1'): return Z_KEY_HOME; // might be with a ~
        case CHAR2_TO_INT('[', '5'): return Z_KEY_PAGE_UP; // might be with a ~
        case CHAR2_TO_INT('[', '6'): return Z_KEY_PAGE_DOWN; // might be with a ~
    }

    return '\033';
}

int z_read_key()
{
    char c = z_wait_for_byte();

    if (c == '\033') {
        return z_read_escape_key();
    }

    return c;
}

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   path implementation
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

Z_String_View z_get_path_extention(Z_String_View path)
{
    int start = path.len - 1;

    while (start > 0 && path.ptr[start] != '.') {
        if (path.ptr[start] == '/') {
            return Z_EMPTY_SV();
        }

        start--;
    }

    if (start == 0) {
        if (path.ptr[0] == '.') {
            return Z_SV(path.ptr + 1, path.len - 1);
        } else {
            return Z_EMPTY_SV();
        }
    }

    Z_String_View extention = {
        .ptr = path.ptr + start,
        .len = path.len - start,
    };

    return extention;
}

Z_String_View z_get_path_basename(Z_String_View pathname)
{
    (void)pathname;
    return Z_EMPTY_SV();
}

Z_String_View z_get_home_path()
{
    const char *home = getenv("HOME");

    if (home == NULL) {
        return Z_CSTR_TO_SV(".");
    }

    return Z_CSTR_TO_SV(home);
}

Z_String z_expand_path(Z_String_View p)
{
    if (p.len == 0) {
        return z_str_new("");
    }

    Z_String_View home = z_get_home_path();

    if (p.ptr[0] == '~') {
        return z_str_new("%.*s%.*s", home.len, home.ptr, p.len - 1, p.ptr + 1);
    }

    return z_str_new("%.*s", p.len, p.ptr);
}

Z_String z_compress_path(Z_String_View p)
{
    Z_String_View home = z_get_home_path();

    if (z_str_n_cmp(p, home, home.len) == 0) {
        return z_str_new("~%.*s", p.len - home.len, p.ptr + home.len);
    }

    return z_str_new("%.*s", p.len, p.ptr);
}

Z_Result z_dir_traverse(const char *dir, bool action(const char *))
{
    DIR *dr = opendir(dir);

    if (dr == NULL) {
        return Z_Err;
    }

    struct dirent *de;

    Z_String full_path = z_str_new("");

    while ((de = readdir(dr))) {

        z_str_clear(&full_path);
        z_str_pushf(&full_path, "%s/%s", dir, de->d_name);

        if (action(full_path.ptr) == false) {
            break;
        }
    }

    z_str_free(&full_path);
    closedir(dr);

    return Z_Ok;
}

bool z_is_extention_equal(Z_String_View pathname, Z_String_View extention)
{
    return z_str_cmp(z_get_path_extention(pathname), extention) == 0;
}

bool z_is_dir(const char *pathname)
{
    struct stat sb;
    stat(pathname, &sb);

    return S_ISDIR(sb.st_mode);
}

bool z_is_regular_file(const char *pathname)
{
    struct stat sb;
    stat(pathname, &sb);

    return S_ISREG(sb.st_mode);
}

bool z_is_path_exists(const char *pathname)
{
    return !access(pathname, F_OK);
}

Z_Result z_write_file(const char *fileName, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    FILE *fp = fopen(fileName, "w");

    if (fp == NULL) {
        return Z_Err;
    }

    vfprintf(fp, fmt, ap);
    va_end(ap);

    return Z_Ok;
}

Z_Result z_append_file(const char *fileName, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    FILE *fp = fopen(fileName, "a");

    if (fp == NULL) {
        return Z_Err;
    }

    vfprintf(fp, fmt, ap);
    va_end(ap);

    return Z_Ok;
}

Z_Result z_read_file(const char *fileName, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    FILE *fp = fopen(fileName, "r");

    if (fp == NULL) {
        return Z_Err;
    }

    if (vfscanf(fp, fmt, ap) == EOF) {
        fclose(fp);
        return Z_Err;
    }

    va_end(ap);
    fclose(fp);

    return Z_Ok;
}

Z_Result z_redirect_fd(int srcFd, const char *destFileName)
{
    int destFd = open(destFileName, O_WRONLY);

    if (destFd == -1) {
        return Z_Err;
    }

    if (dup2(destFd, srcFd) == -1) {
        close(destFd);
        return Z_Err;
    }

    close(destFd);

    return Z_Ok;
}

Z_Result z_popen2(char *pathname, char *argv[], FILE *ppipe[2])
{
    int output[2];
    int input[2];

    if (pipe(output) == -1 || pipe(input) == -1) {
        return Z_Err;
    }

    int pid = fork();

    if (pid == -1) {
        return Z_Err;
    }

    if (pid) {
        // parent
        close(output[Z_Pipe_Mode_Write]);
        ppipe[Z_Pipe_Mode_Write] = fdopen(input[Z_Pipe_Mode_Write], "w");
        ppipe[Z_Pipe_Mode_Read] = fdopen(output[Z_Pipe_Mode_Read], "r");
    } else {
        // child
        dup2(input[Z_Pipe_Mode_Read], STDIN_FILENO);
        dup2(output[Z_Pipe_Mode_Write], STDOUT_FILENO);
        close(input[Z_Pipe_Mode_Write]);
        close(input[Z_Pipe_Mode_Read]);
        close(output[Z_Pipe_Mode_Write]);
        close(output[Z_Pipe_Mode_Read]);
        execvp(pathname, argv);
        exit(EXIT_FAILURE);
    }

    return Z_Ok;
}

bool z_mkdir(const char *pathname)
{
    int status = mkdir(pathname, 0777);

    if (status == 0) {
        return true;
    } else if (errno == EEXIST) {
        z_print_warning("cannot create directory ‘%s’: File exists", pathname);
    } else {
        z_print_error("%s", strerror(errno));
    }

    return false;
}

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   string implementation
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

Z_String z_str_new(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    Z_String s = z_str_new_va(fmt, ap);
    va_end(ap);

    return s;
}

Z_String z_str_new_va(const char *fmt, va_list ap)
{
    Z_String s = {0};
    z_str_pushf_va(&s, fmt, ap);

    return s;
}

void z_str_pushf(Z_String *s, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    z_str_pushf_va(s, fmt, ap);
    va_end(ap);
}

void z_str_pushf_va(Z_String *s, const char *fmt, va_list ap)
{
    int len = z_get_fmt_size_va(fmt, ap);
    z_ensure_capacity(s, s->len + len + 1);

    va_list ap1;
    va_copy(ap1, ap);
    vsnprintf(s->ptr + s->len, len + 1, fmt, ap1);
    va_end(ap1);

    s->len = s->len + len;
}

void z_str_push(Z_String *dst, Z_String_View src)
{
    z_str_pushf(dst, "%.*s", src.len, src.ptr);
}

void z_str_push_c(Z_String *s, char c)
{
    z_ensure_capacity(s, s->len + 1);
    s->ptr[s->len++] = c;
    z_null_terminate(s);
}

char z_str_pop_c(Z_String *s)
{
    return s->ptr[--s->len];
}

char z_str_top_c(Z_String_View s)
{
    return s.ptr[s.len - 1];
}

int z_str_cmp(Z_String_View s1, Z_String_View s2)
{
    if (s1.len > s2.len) {
        return 1;
    } else if (s1.len < s2.len) {
        return -1;
    } else {
        return memcmp(s1.ptr, s2.ptr, s1.len);
    }
}

int z_str_n_cmp(Z_String_View s1, Z_String_View s2, int n)
{
    assert(s1.len >= n && s2.len >= n);

    return memcmp(s1.ptr, s2.ptr, n);
}

void z_str_replace(Z_String *s, Z_String_View target, Z_String_View replacement);

void z_str_trim(Z_String *s);
void z_str_trim_cset(Z_String *s, Z_String_View cset);

Z_String_View z_str_view_trim(Z_String_View s);
Z_String_View z_str_view_trim_cset(Z_String_View s, Z_String_View cset);

void z_str_print(Z_String_View s)
{
    printf("%.*s", s.len, s.ptr);
}

void z_str_println(Z_String_View s)
{
    printf("%.*s\n", s.len, s.ptr);
}

void z_str_free(Z_String *s)
{
    free(s->ptr);
    s->ptr = NULL;
    s->len = 0;
    s->capacity = 0;
}

void z_str_clear(Z_String *s)
{
    s->len = 0;
    z_null_terminate(s);
}

Z_String z_read_whole_file(const char *pathname, Z_Result *result)
{
    FILE *fp = fopen(pathname, "r");

    if (fp == NULL) {
        *result = Z_Err;
        return (Z_String){0};
    }

    int file_size = z_get_file_size(fp);

    Z_String s;
    s.capacity = file_size + 1;
    s.ptr = malloc(sizeof(char) * s.capacity);
    s.len = fread(s.ptr, sizeof(char), file_size, fp);
    s.ptr[s.len] = '\0';

    fclose(fp);
    *result = Z_Ok;

    return s;
}

//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//
//
//   cmd implementation
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

bool _z_should_rebuild(const char *target, ...)
{
    va_list ap;
    va_start(ap, target);
    bool should_rebuild = z_should_rebuild_va(target, ap);
    va_end(ap);

    return should_rebuild;
}

bool z_should_rebuild_va(const char *target, va_list ap)
{
    va_list ap1;
    va_copy(ap1, ap);

	struct stat target_stat;
	struct stat dependency_stat;

    if (stat(target, &target_stat)) {
        return true;
    }

    const char *dependency = va_arg(ap, const char *);

    while (dependency) {
        if (stat(dependency, &dependency_stat)) {
            z_print_error("cannot access '%s': No such file", dependency);
            va_end(ap1);
            return false;
        } else if (target_stat.st_mtim.tv_sec < dependency_stat.st_mtim.tv_sec) {
            va_end(ap1);
            return true;
        }

        dependency = va_arg(ap, const char *);
    }

    return false;
}

void z_rebuild_yourself(const char *src_pathname, char **argv)
{
    if (!z_should_rebuild(argv[0], src_pathname, __FILE__)) {
        return;
    }

    int status;
    status = z_run_async("cc", src_pathname, "-o", argv[0]);

    if (status != 0) {
        exit(status);
    }

	status = execvp(argv[0], argv);
    exit(1);
}

void z_cmd_init(Z_Cmd *cmd)
{
    cmd->ptr = NULL;
    cmd->len = 0;
    cmd->capacity = 0;
}

void _z_cmd_append(Z_Cmd *cmd, ...)
{
	va_list ap;
    va_start(ap, cmd);
    z_cmd_append_va(cmd, ap);
	va_end(ap);
}

void z_cmd_append_va(Z_Cmd *cmd, va_list ap)
{
	va_list ap1;
    va_copy(ap1, ap);

	const char *arg = va_arg(ap1, const char *);

	while (arg) {
        z_ensure_capacity(cmd, cmd->len + 1);
		cmd->ptr[cmd->len++] = strdup(arg);
		arg = va_arg(ap1, const char *);
	}

	va_end(ap1);
}

void z_cmd_print_arg(const char *arg)
{
	if (strchr(arg, ' ')) {
		printf("'%s'", arg);
	} else {
		printf("%s", arg);
	}
}

void z_cmd_print(const Z_Cmd *cmd)
{
	printf("[" Z_COLOR_GREEN "CMD" Z_COLOR_RESET "]");

	for (int i = 0; i < cmd->len; i++) {
		printf(" ");
		z_cmd_print_arg(cmd->ptr[i]);
	}

	printf("\n");
}

int z_cmd_run_async(Z_Cmd *cmd)
{
    z_null_terminate(cmd);
	z_cmd_print(cmd);

	pid_t pid = fork();
	int status = 0;

	if (pid == -1) {
		z_print_error("fork couln't create child");
	} else if (pid == 0) {
		exit(execvp(cmd->ptr[0], cmd->ptr));
	} else {
		waitpid(pid, &status, 0);
	}

	if (status != 0) {
		z_print_error(Z_COLOR_RED "exited abnormally " Z_COLOR_RESET
                "with code " Z_COLOR_RED "%d" Z_COLOR_RESET, status);
	}

	return status;
}

int _z_run_async(const char *arg, ...)
{
	va_list ap;
	va_start(ap, arg);

	Z_Cmd cmd;
	z_cmd_init(&cmd);
	z_cmd_append(&cmd, arg);
	z_cmd_append_va(&cmd, ap);

	int status = z_cmd_run_async(&cmd);

    z_cmd_free(&cmd);
	va_end(ap);

	return status;
}

void z_cmd_free(Z_Cmd *cmd)
{
    for (int i = 0; i < cmd->len; i++) {
        free(cmd->ptr[i]);
    }

    free(cmd->ptr);
}

void z_cmd_clear(Z_Cmd *cmd)
{
    for (int i = 0; i < cmd->len; i++) {
        free(cmd->ptr[i]);
    }

    cmd->len = 0;
}
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $

#endif // end implementation
#endif // end header

//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
//       $       $       $       $       $       $        $        $
//   $       $       $       $       $       $       $        $        $
