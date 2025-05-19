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

typedef enum { Z_Ok, Z_Err, } Z_Result;

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

#define z_ensure_capacity(da, cap)                                                     \
	do {                                                                               \
		int new_capacity = z_max((cap), (da)->capacity * Z_DEFAULT_GROWTH_RATE);       \
		if (new_capacity > (da)->capacity) {                                           \
			(da)->capacity = new_capacity;                                             \
			(da)->ptr = realloc((da)->ptr, sizeof((da)->ptr[0]) * (da)->capacity);     \
		}                                                                              \
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

#define CTRL_KEY(k)				((k) & 0x1f)

#define Z_COLOR_RESET			"\033[0m"

#define Z_COLOR_RED		        "\033[0;31m"
#define Z_COLOR_GREEN	        "\033[0;32m"
#define Z_COLOR_YELLOW          "\033[0;33m"
#define Z_COLOR_BLUE	        "\033[0;34m"
#define Z_COLOR_MAGENTA         "\033[0;35m"
#define Z_COLOR_CYAN	        "\033[0;36m"
#define Z_COLOR_WHITE	        "\033[0;37m"
#define Z_COLOR_GRAY	        "\033[0;90m"

#define Z_COLOR_BOLD_RED		"\033[1;91m"
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

Z_Result z_enable_raw_mode(int vminKeys, int vtime);
Z_Result z_disable_raw_mode();

void z_disable_line_wrap();
void z_enbale_line_wrap();

void z_hide_cursor();
void z_show_cursor();

void z_set_cursor_style(Z_CURSOR_STYLE style);

Z_Result z_get_cursor_pos(int *x, int *y);
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

#define Z_VECTOR_DECLARE(name, T, prefix)							   \
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
} name;

#define Z_MAT_AT(mat, x, y) ((mat)->ptr[(y) * (mat)->x + x])

#define Z_MAT_INIT(mat, x, y)                                     \
	do {                                                          \
		(mat)->ptr = malloc(sizeof((mat)->ptr[0]) * (x) * (y));   \
		(mat)->x = x;                                             \
		(mat)->y = y;                                             \
	} while (0)

#define Z_MAT_FREE(mat) free((mat)->ptr)

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
} Z_Pipe_Mode;

const char *z_get_path_extention(const char *path);
const char *z_get_home_path();
char *z_expand_path(const char *pathname);
char *z_compress_path(const char *pathname);
bool z_is_extention_equal(const char *path, const char *extention);

Z_Result z_dir_traverse(const char *dir, bool action(const char *));

bool z_is_dir(const char *pathname);
bool z_is_regular_file(const char *pathname);
bool z_is_path_exists(const char *pathname);

Z_Result z_write_file(const char *pathname, const char *fmt, ...);
Z_Result z_append_file(const char *pathname, const char *fmt, ...);
Z_Result z_read_file(const char *pathname, const char *fmt, ...);

Z_Result z_redirect_fd(int src_fd, const char *dst_pathname);
Z_Result z_popen2(char *path, char *argv[], FILE *ppipe[2]);

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
} Z_Str;

typedef Z_Str Z_Str_Slice;

Z_Str z_str_new(const char *fmt, ...);
Z_Str z_str_new_va(const char *fmt, va_list ap);

void z_str_init(Z_Str *s, const char *fmt, ...);
void z_str_init_va(Z_Str *s, const char *fmt, va_list ap);

void z_str_push(Z_Str *s, const char *fmt, ...);
void z_str_push_va(Z_Str *s, const char *fmt, va_list ap);
void z_str_push_c(Z_Str *s, char c);
char z_str_top_c(Z_Str *s);
char z_str_pop_c(Z_Str *s);
void z_str_push_str(Z_Str *dst, const Z_Str_Slice src);

int z_str_len(Z_Str_Slice s);
bool z_str_is_empty(Z_Str_Slice s);

Z_Str_Slice z_str_tok_init(Z_Str_Slice s);
Z_Result z_str_tok_next(Z_Str_Slice s, Z_Str_Slice *slice, const char *delim);

void z_str_replace(Z_Str *s, const char *target, const char *replacement);
void z_str_trim(Z_Str *s);
void z_str_trim_cset(Z_Str *s, const char *cset);

void z_str_print(Z_Str_Slice s);
void z_str_println(Z_Str_Slice s);
void z_str_free(Z_Str *s);
Z_Result z_read_whole_file(Z_Str *s, const char *pathname);

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

Z_Result z_dir_traverse(const char *dir, bool action(const char *))
{
	struct dirent *de;
	DIR *dr = opendir(dir);

	if (dr == NULL) {
		return Z_Err;
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

	return Z_Ok;
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

	return Z_Ok;
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

Z_Str z_str_new(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	Z_Str s = z_str_new_va(fmt, ap);
	va_end(ap);

	return s;
}

Z_Str z_str_new_va(const char *fmt, va_list ap)
{
	Z_Str s = {0};
	z_str_push_va(&s, fmt, ap);

	return s;
}

void z_str_init(Z_Str *s, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	z_str_init_va(s, fmt, ap);
	va_end(ap);
}

void z_str_init_va(Z_Str *s, const char *fmt, va_list ap)
{
	s->ptr = NULL;
	s->len = 0;
	s->capacity = 0;

	z_str_push_va(s, fmt, ap);
}

void z_str_push(Z_Str *s, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	z_str_push_va(s, fmt, ap);
	va_end(ap);
}

void z_str_push_va(Z_Str *s, const char *fmt, va_list ap)
{
	int len = z_get_fmt_size_va(fmt, ap);
	z_ensure_capacity(s, s->len + len + 1);

	va_list ap1;
	va_copy(ap1, ap);
	vsnprintf(s->ptr + s->len, len + 1, fmt, ap1);
	va_end(ap1);

	s->len = s->len + len;
}

void z_str_push_c(Z_Str *s, char c)
{
	z_ensure_capacity(s, s->len + 1);
	s->ptr[s->len++] = c;
}

char z_str_top_c(Z_Str *s)
{
	return s->ptr[s->len - 1];
}

char z_str_pop_c(Z_Str *s)
{
	return s->ptr[--s->len];
}

void z_str_push_str(Z_Str *dst, const Z_Str_Slice src)
{
	z_str_push(dst, "%.*s", src.len, src.ptr);
}

int z_str_len(Z_Str_Slice s)
{
	return s.len;
}

bool z_str_is_empty(Z_Str_Slice s)
{
	return s.len == 0;
}

Z_Str_Slice z_str_tok_init(Z_Str_Slice s)
{
	Z_Str_Slice slice = {
		.len = 0,
		.ptr = s.ptr,
	};

	return slice;
}

Z_Result z_str_tok_next(Z_Str_Slice s, Z_Str_Slice *slice, const char *delim)
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
		return Z_Err;
	}

	slice->ptr = start;
	slice->len = end - start;

	return Z_Ok;
}

void z_str_replace(Z_Str *s, const char *target, const char *replacement); // TODO: implement
void z_str_trim(Z_Str *s); // TODO: implement
void z_str_trim_cset(Z_Str *s, const char *cset); // TODO: implement

void z_str_print(Z_Str_Slice s)
{
	printf("%.*s", s.len, s.ptr);
}

void z_str_println(Z_Str_Slice s)
{
	printf("%.*s\n", s.len, s.ptr);
}

void z_str_free(Z_Str *s)
{
	free(s->ptr);
}

Z_Result z_read_whole_file(Z_Str *s, const char *pathname)
{
	FILE *fp = fopen(pathname, "r");

	if (fp == NULL) {
		return Z_Err;
	}

	int file_size = z_get_file_size(fp);

	s->capacity = file_size + 1;
	s->ptr = malloc(sizeof(char) * s->capacity);
	s->len = fread(s->ptr, sizeof(char), file_size, fp);
	s->ptr[s->len] = '\0';

	fclose(fp);

	return Z_Ok;
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
