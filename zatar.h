/*
 *           --Libzatar--
 *
 *         Data Structures:
 *  	      - Vector
 *
 *
 *
 *  	      #define LIBZATAR_IMPLEMENTATION
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
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

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

typedef enum { Ok, Err, } z_result;

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

#define z_da_append(da, item, grow_rate)                                            \
	do {                                                                            \
		(da)->len++;                                                                \
		if ((da)->len >= (da)->capacity) {                                          \
			(da)->capacity = z_max(1, (da)->capacity * (grow_rate));                \
			(da)->ptr = realloc((da)->ptr, sizeof((da)->ptr[0]) * (da)->capacity);  \
		}                                                                           \
		(da)->ptr[(da)->len - 1] = item;                                            \
	} while (0)

#define z_da_append_many(da, items, len, grow_rate)                                 \
	do {                                                                            \
		for (int i = 0; i < (len); i++) {                                           \
			z_da_append(da, items[i], grow_rate);                                   \
		}                                                                           \
	} while (0)


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

#define CTRL_KEY(k) ((k) & 0x1f)

#define C0  "\033[0m"     /*  RESET        */

#define C1  "\033[0;31m"  /*  RED          */
#define C2  "\033[0;32m"  /*  GREEN        */
#define C3  "\033[0;33m"  /*  YELLOW       */
#define C4  "\033[0;34m"  /*  BLUE         */
#define C5  "\033[0;035m" /*  MAGENTA      */
#define C6  "\033[0;36m"  /*  CYAN         */
#define C7  "\033[0;37m"  /*  WHITE        */
#define C8  "\033[0;90m"  /*  GRAY         */

#define B1  "\033[1;91m"  /*  BOLD RED     */
#define B2  "\033[1;92m"  /*  BOLD GREEN   */
#define B3  "\033[1;93m"  /*  BOLD YELLOW  */
#define B4  "\033[1;94m"  /*  BOLD BLUE    */
#define B5  "\033[1;95m"  /*  BOLD MAGENTA */
#define B6  "\033[1;96m"  /*  BOLD CYAN    */
#define B7  "\033[1;97m"  /*  BOLD WHITE   */
#define B8  "\033[1;90m"  /*  BOLD GRAY    */

#define Z_KEY_EMPTY       999
#define Z_KEY_ARROW_LEFT  1000
#define Z_KEY_ARROW_RIGHT 1001
#define Z_KEY_ARROW_UP    1002
#define Z_KEY_ARROW_DOWN  1003
#define Z_KEY_PAGE_UP     1007
#define Z_KEY_PAGE_DOWN   1008
#define Z_KEY_DELETE      1004
#define Z_KEY_HOME        1005
#define Z_KEY_END         1006

typedef enum {
	Z_CURSOR_STYLE_BLOCK_STEADY       = 0,
	Z_CURSOR_STYLE_BLOCK_BLINKING     = 1,
	Z_CURSOR_STYLE_UNDERLINE_BLINKING = 3,
	Z_CURSOR_STYLE_UNDERLINE_STEADY   = 4,
	Z_CURSOR_STYLE_BEAM_STEADY        = 6,
	Z_CURSOR_STYLE_BEAM_BLINKING      = 5,
} Z_CURSOR_STYLE;

z_result z_enable_raw_mode(int vminKeys, int vtime);
z_result z_disable_raw_mode();

void z_disable_line_wrap();
void z_enbale_line_wrap();

void z_hide_cursor();
void z_show_cursor();

void z_set_cursor_style(Z_CURSOR_STYLE style);

z_result z_get_cursor_pos(int *x, int *y);
void z_set_cursor_pos(int x, int y);

void z_set_cursor_x(int x);

void z_cursor_up(int n);
void z_cursor_down(int n);
void z_cursor_right(int n);
void z_cursor_left(int n);

// unsupported on some terminals
void z_save_cursor_pos();
void z_restore_cursor_pos();

void z_enter_alternative_screen();
void z_exit_alternative_screen();

void z_clear_line();
void z_clear_screen();

void z_update_screen();

z_result z_get_screen_size_by_cursor(int *width, int *height);
z_result z_get_screen_size_by_ioctl(int *width, int *height);
z_result z_get_screen_size(int *width, int *height);

z_result z_register_change_in_window_size(void function(int));

z_result z_enable_full_buffering(FILE *fp);

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

#define VECTOR_DECLARE(name, type)                                     \
                                                                       \
typedef struct {                                                       \
	type *ptr;                                                         \
	int len;                                                           \
	int capacity;                                                      \
} name;                                                                \
                                                                       \
name *name##_new();                                                    \
type name##_at(name *v, int i);                                        \
void name##_add(name *v, type data);                                   \
type name##_remove_last(name *v);                                      \
int name##_len(const name *v);                                         \
bool name##_is_empty(const name *v);                                   \
void name##_for_each(name *v, void function(type));                    \
void name##_free(name *v, void free_function(type));                   \
void name##_print(const name *v, void print_data(type))



#define Z_VECTOR_IMPLEMENT(name, type)                                   \
                                                                       \
name *name##_new()                                                     \
{                                                                      \
	name *v = malloc(sizeof(name));                                    \
	v->ptr = NULL;                                                     \
	v->len = 0;                                                        \
	v->capacity = 0;                                                   \
                                                                       \
	return v;                                                          \
}                                                                      \
                                                                       \
type name##_at(name *v, int i)                                         \
{                                                                      \
	return v->ptr[i];                                                  \
}                                                                      \
                                                                       \
void name##_add(name *v, type data)                                    \
{                                                                      \
	z_da_append(v, data, Z_DEFAULT_GROWTH_RATE)                        \
}                                                                      \
                                                                       \
type name##_remove_last(name *v)                                       \
{                                                                      \
	return v->ptr[--v->len];                                           \
}                                                                      \
                                                                       \
int name##_len(const name *v)                                          \
{                                                                      \
	return v->len;                                                     \
}                                                                      \
                                                                       \
bool name##_is_empty(const name *v)                                    \
{                                                                      \
	return v->len == 0;                                                \
}                                                                      \
                                                                       \
void name##_for_each(name *v, void function(type))                     \
{                                                                      \
	for (int i = 0; i < v->len; i++) {                                 \
		function(v->ptr[i]);                                           \
	}                                                                  \
}                                                                      \
                                                                       \
void name##_free(name *v, void free_function(type))                    \
{                                                                      \
	if (free_function) {                                               \
		name##_for_each(v, free_function);                             \
	} else {                                                           \
		free(v->ptr);                                                  \
	}                                                                  \
                                                                       \
	free(v);                                                           \
}                                                                      \
                                                                       \
void name##_print(const name *v, void print_data(type))                \
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
//   matrix header
//
//
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *
//       *       *       *       *       *       *        *        *
//   *       *       *       *       *       *       *        *        *

#define Z_MAT_DECLARE(name, type) \
typedef struct {                  \
	type *ptr;                    \
	int x;                        \
	int y;                        \
} name;

#define Z_MAT_AT(mat, x, y) ((mat)->ptr[(y) * (mat)->x + x])

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
	Read = 0,
	Write = 1,
} z_PipeMode;

const char *z_get_path_extention(const char *path);
const char *z_get_home_path();
char *z_expand_path(const char *pathname);
char *z_compress_path(const char *pathname);
bool z_is_extention_equal(const char *path, const char *extention);

z_result z_dir_traverse(const char *dir, bool action(const char *));

bool z_is_dir(const char *pathname);
bool z_is_regular_file(const char *pathname);
bool z_is_path_exists(const char *pathname);

z_result z_write_file(const char *pathname, const char *fmt, ...);
z_result z_append_file(const char *pathname, const char *fmt, ...);
z_result z_read_file(const char *pathname, const char *fmt, ...);

z_result z_redirect_fd(int src_fd, const char *dst_pathname);
z_result z_popen2(char *path, char *argv[], FILE *ppipe[2]);

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
} z_str;

typedef z_str z_str_slice;

z_str z_str_new(const char *fmt, ...);
z_str z_str_new_va(const char *fmt, va_list ap);

void z_str_push(z_str *s, const char *fmt, ...);
void z_str_push_va(z_str *s, const char *fmt, va_list ap);
void z_str_push_c(z_str *s, char c);
char z_str_top_c(z_str *s);
char z_str_pop_c(z_str *s);
void z_str_push_str(z_str *dst, const z_str_slice src);

int z_str_len(z_str_slice s);
bool z_str_is_empty(z_str_slice s);

z_str_slice z_str_tok_init(z_str_slice s, const char *delim);
z_result z_str_tok_next(z_str_slice s, z_str_slice *slice, const char *delim);

void z_str_replace(z_str *s, const char *target, const char *replacement);
void z_str_trim(z_str *s);
void z_str_trim_cset(z_str *s, const char *cset);

void z_str_print(z_str_slice s);
void z_str_println(z_str_slice s);
void z_str_free(z_str *s);
z_str z_read_whole_file(FILE *fp);


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

z_result z_enable_raw_mode(int vminKeys, int vtime)
{
	if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
		return Err;
	}

	struct termios raw = original_termios;
	raw.c_cc[VMIN] = vminKeys;
	raw.c_cc[VTIME] = vtime;
	raw.c_cflag |= (CS8);
	raw.c_oflag &= ~(OPOST);
	raw.c_iflag &= ~(IXON | ICRNL | ISTRIP | INPCK | BRKINT);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
		return Err;
	}

	return Ok;
}

z_result z_disable_raw_mode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1) {
		return Err;
	}

	return Ok;
}

z_result z_get_cursor_pos(int *x, int *y)
{
	printf("\033[6n");

	if (scanf("\033[%d;%dR", y, x) == 2) {
		return Ok;
	}

	return Err;
}

#define z_set_cursor_style(style)         printf("\033[%d q", (int)(style))
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
#define z_save_cursor_pos()               printf("\033[s")
#define z_restore_cursor_pos()            printf("\033[u")
#define z_enter_alternative_screen()      printf("\033[?1049h")
#define z_exit_alternative_screen()       printf("\033[?1049l")
#define z_clear_line()                    printf("\033[K")
#define z_clear_screen()                  printf("\033[2J")

z_result z_get_screen_size_by_cursor(int *width, int *height)
{
	z_set_cursor_pos(999, 999);

	return z_get_cursor_pos(width, height);
}

z_result z_get_screen_size_by_ioctl(int *width, int *height)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0) {
		return Err;
	}

	*width = ws.ws_col;
	*height = ws.ws_row;

	return Ok;
}

z_result z_get_screen_size(int *width, int *height)
{
	if (z_get_screen_size_by_ioctl(width, height) == Ok) {
		return Ok;
	}

	return z_get_screen_size_by_cursor(width, height);
}

z_result z_register_change_in_window_size(void funciton(int))
{
	struct sigaction sa;
	sa.sa_handler = funciton;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGWINCH, &sa, NULL) == -1) {
		return Err;
	}

	return Ok;
}

z_result z_enable_full_buffering(FILE *fp)
{
	// do not flush on '\n'
	if (setvbuf(fp, NULL, _IOFBF, BUFSIZ) != 0) {
		return Err;
	}

	return Ok;
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

const char *z_get_path_extention(const char *pathname)
{
	const char *last_dot = strrchr(pathname, '.');

	if (last_dot == NULL) {
		return pathname;
	}

	return last_dot + 1;
}

const char *z_get_home_path()
{
	const char *home = getenv("HOME");

	if (home == NULL) {
		return ".";
	}

	return home;
}

char *z_expand_path(const char *pathname)
{
	if (pathname[0] == '~') {

		const char *home = z_get_home_path();
		int size = z_get_fmt_size("%s%s", home, pathname + 1) + 1;
		char *dst = malloc(sizeof(char) * size);
		snprintf(dst, size, "%s%s", home, pathname + 1);

		return dst;
	}

	return strdup(pathname);
}

char *z_compress_path(const char *pathname)
{
	const char *home = z_get_home_path();
	int home_len = strlen(home);

	if (strncmp(home, pathname, home_len) == 0) {
		int len = strlen(pathname) - home_len + 1;
		char *ret = malloc(sizeof(char) * len);
		ret[0] = '~';
		strcpy(ret + 1, pathname + home_len);

		return ret;
	}

	return strdup(pathname);
}

z_result z_dir_traverse(const char *dir, bool action(const char *))
{
	struct dirent *de;
	DIR *dr = opendir(dir);

	if (dr == NULL) {
		return Err;
	}

	char fullPath[PATH_MAX];

	while ((de = readdir(dr))) {

		const char *file = de->d_name;

		int len = snprintf(fullPath, PATH_MAX, "%s/%s", dir, file);
		fullPath[len] = '\0';

		if (action(fullPath) == false) {
			break;
		}
	}

	closedir(dr);

	return Ok;
}

bool z_is_extention_equal(const char *pathname, const char *extention)
{
	return strcmp(z_get_path_extention(pathname), extention) == 0;
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

z_result z_write_file(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "w");

	if (fp == NULL) {
		return Err;
	}

	vfprintf(fp, fmt, ap);
	va_end(ap);

	return Ok;
}

z_result z_append_file(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "a");

	if (fp == NULL) {
		return Err;
	}

	vfprintf(fp, fmt, ap);
	va_end(ap);

	return Ok;
}

z_result z_read_file(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "r");

	if (fp == NULL) {
		return Err;
	}

	if (vfscanf(fp, fmt, ap) == EOF) {
        fclose(fp);
		return Err;
    }

	va_end(ap);
    fclose(fp);

	return Ok;
}

z_result z_redirect_fd(int srcFd, const char *destFileName)
{
	int destFd = open(destFileName, O_WRONLY);

	if (destFd == -1) {
		return Err;
	}

	if (dup2(destFd, srcFd) == -1) {
		close(destFd);
		return Err;
	}

	close(destFd);

	return Ok;
}

z_result z_popen2(char *pathname, char *argv[], FILE *ppipe[2])
{
	int output[2];
	int input[2];

	if (pipe(output) == -1 || pipe(input) == -1) {
		return Err;
	}

	int pid = fork();

	if (pid == -1) {
		return Err;
	}

	if (pid) {
		// parent
		close(output[Write]);
		ppipe[Write] = fdopen(input[Write], "w");
		ppipe[Read] = fdopen(output[Read], "r");
	} else {
		// child
		dup2(input[Read], STDIN_FILENO);
		dup2(output[Write], STDOUT_FILENO);
		close(input[Write]);
		close(input[Read]);
		close(output[Write]);
		close(output[Read]);
		execvp(pathname, argv);
		exit(EXIT_FAILURE);
	}

	return Ok;
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

z_str z_str_new(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	z_str s = z_str_new_va(fmt, ap);
	va_end(ap);

	return s;
}

z_str z_str_new_va(const char *fmt, va_list ap)
{
	z_str s = {0};
	z_str_push_va(&s, fmt, ap);

	return s;
}

void z_str_push(z_str *s, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	z_str_push_va(s, fmt, ap);
	va_end(ap);
}

void z_str_push_va(z_str *s, const char *fmt, va_list ap)
{
	va_list ap1;
	va_copy(ap1, ap);

	int len = z_get_fmt_size_va(fmt, ap);
	char buf[len + 1];

	vsnprintf(buf, len + 1, fmt, ap1);
	z_da_append_many(s, buf, len + 1, Z_DEFAULT_GROWTH_RATE);

	va_end(ap1);
}

void z_str_push_c(z_str *s, char c)
{
	s->ptr[s->len] = c;
	z_da_append(s, '\0', Z_DEFAULT_GROWTH_RATE);
}

char z_str_top_c(z_str *s)
{
	return s->ptr[s->len - 1];
}

char z_str_pop_c(z_str *s)
{
	return s->ptr[--s->len];
}

void z_str_push_str(z_str *dst, const z_str_slice src)
{
	z_str_push(dst, "%.*s", src.len, src.ptr);
}

int z_str_len(z_str_slice s)
{
	return s.len;
}

bool z_str_is_empty(z_str_slice s)
{
	return s.len == 0;
}

z_str_slice z_str_tok_init(z_str_slice s, const char *delim)
{
	z_str_slice slice = {
		.len = 0,
		.ptr = s.ptr,
	};

	return slice;
}

z_result z_str_tok_next(z_str_slice s, z_str_slice *slice, const char *delim)
{
	char *start = slice->ptr + slice->len;
	char *str_end = s.ptr + s.len;

	while (start < str_end && strchr(delim, *start) != NULL) {
		start++;
	}

	char *end = start;

	while (end < str_end && strchr(delim, *end) == NULL) {
		end++;
	}

	if (start == str_end) {
		return Err;
	}

	slice->ptr = start;
	slice->len = end - start;

	return Ok;
}

void z_str_replace(z_str *s, const char *target, const char *replacement); // TODO: implement
void z_str_trim(z_str *s); // TODO: implement
void z_str_trim_cset(z_str *s, const char *cset); // TODO: implement

void z_str_print(z_str_slice s)
{
	printf("%.*s", s.len, s.ptr);
}

void z_str_println(z_str_slice s)
{
	printf("%.*s\n", s.len, s.ptr);
}

void z_str_free(z_str *s)
{
	free(s->ptr);
}

z_str z_read_whole_file(FILE *fp)
{
	z_str s;

	int file_size = z_get_file_size(fp);
	s.ptr = malloc(sizeof(char) * (file_size + 1));
	s.len = fread(s.ptr, sizeof(char), file_size, fp);
	s.ptr[s.len] = '\0';

	return s;
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

#define AVL_TREE_DECLARE(name, key_type, data_type)                               \
                                                                                  \
typedef struct name##_node {                                                      \
	struct name##_node *left;                                                     \
	struct name##_node *right;                                                    \
	key_type key;                                                                 \
	data_type data;                                                               \
	int height;                                                                   \
} name##_node;                                                                    \
                                                                                  \
name##_node *name##_node_new(key_type key, data_type data);                       \
name##_node *name##_get_min(name##_node *root);                                   \
name##_node *name##_get_max(name##_node *root);                                   \
                                                                                  \
name##_node *name##_find_node(const name##_node *root,                            \
							key_type key,                                         \
							int cmp_key(key_type, key_type));                     \
                                                                                  \
bool name##_is_exists(const name##_node *root,                                    \
				   	 key_type key,                                                \
					 int cmp_keys(key_type, key_type));                           \
                                                                                  \
data_type name##_find(const name##_node *root,                                    \
				     key_type key,                                                \
					 int cmp_keys(key_type, key_type));                           \
                                                                                  \
void name##_insert(name##_node **root,                                            \
	              key_type key,                                                   \
	              data_type data,                                                 \
	              int cmp_keys(key_type, key_type));                              \
                                                                                  \
void name##_remove(name##_node **root,                                            \
		          key_type key,                                                   \
				  int cmp_keys(key_type, key_type),                               \
				  void free_key(key_type),                                        \
				  void free_data(data_type));                                     \
                                                                                  \
void name##_update(name##_node *root,                                             \
				  key_type key,                                                   \
				  int cmp_keys(key_type, key_type),                               \
				  void free_data(data_type),                                      \
				  data_type new_data);                                            \
                                                                                  \
void name##_order_traverse(const name##_node *root,                               \
		                  void action(key_type key, data_type data, void *arg),   \
						  void *arg);                                             \
                                                                                  \
void name##_print(name##_node *root,                                              \
		         void print(key_type key, data_type data, void *arg),             \
				 void *arg,                                                       \
				 int padding);                                                    \
                                                                                  \
void name##_free(name##_node *root,                                               \
		        void free_key(key_type),                                          \
				void free_data(data_type));

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
