/*  libzatar
	Libraries in this file:
	- path
	- cursor
*/

#ifndef LIBZATAR_H
#define LIBZATAR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define ADDRESS(element) (&((typeof(element)){element}))
#define DUP(value) memdup(ADDRESS(value), sizeof(value))
#define IN_RANGE(min, x, max) ((min) <= (x) && (x) <= (max))

typedef enum {
	Ok = 0,
	Err = -1,
} Result;

/*********************************************
                    String
**********************************************/

u64 getFileSize(FILE *fp);
i32 getFmtSize(const char *fmt, ...);
i32 getFmtSizeVa(const char *fmt, va_list ap);


/*********************************************
                    Cursor
**********************************************/
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

enum {
    EMPTY_KEY = 999,

	ARROW_LEFT = 1000,
	ARROW_RIGHT = 1001,
	ARROW_UP = 1002,
	ARROW_DOWN = 1003,

	PAGE_UP = 1007,
	PAGE_DOWN = 1008,

	DELETE = 1004,
	HOME = 1005,
	END = 1006,
};

typedef enum {
	BLOCK_STEADY = 0,
	BLOCK_BLINKING = 1,
	UNDERLINE_BLINKING = 3,
	UNDERLINE_STEADY = 4,
	BEAM_STEADY = 6,
	BEAM_BLINKING = 5,
} CURSOR_STYLE;

Result enableRawMode(int vminKeys, int vtime);
Result disableRawMode();

void disableLineWrap();
void enableLineWrap();

void hideCursor();
void showCursor();

void setCursorStyle(CURSOR_STYLE style);

Result getCursorPos(int *x, int *y);
void setCursorPos(int x, int y);

void setCursorX(int x);

void cursorUp(int n);
void cursorDown(int n);
void cursorRight(int n);
void cursorLeft(int n);

// unsupported on some terminals
void saveCursorPos();
void restoreCursorPos();

void enterAlternativeScreen();
void exitAlternativeScreen();

void clearLine();
void clearScreen();

void updateScreen();

Result getScreenSizeByCursor(int *width, int *height);
Result getScreenSizeByIoctl(int *width, int *height);
Result getScreenSize(int *width, int *height);

Result registerChangeInWindowSize(void (*funciton)(int));

Result enableFullBuffering(FILE *fp);

int waitForByte();
int readEscapeKey();
int readKey();


/*********************************************
                    Path
**********************************************/

#include <dirent.h>
#include <errno.h>
#include <sys/types.h>

typedef enum {
	Read = 0,
	Write = 1,
} PipeMode;

const char *getPathExtention(const char *path);
const char *getHomePath();
char *expandPath(const char *path);
void compressPath(char *path);
bool isExtentionEqual(const char *path, const char *extention);

int dirTraverse(const char *dir, bool (*action)(const char *));
int traverseFile(const char *fileName, int bufSize, bool (*action)(char[bufSize]));

bool isDir(const char *path);
bool isRegularFile(const char *fileName);
bool isPathExists(const char *path);

int echoFileWrite(const char *fileName, const char *fmt, ...);
int echoFileAppend(const char *fileName, const char *fmt, ...);
int readFile(const char *fileName, const char *fmt, ...);
int redirectFd(int srcFd, const char *destFileName);
int popen2(char *path, char *argv[], FILE *ppipe[2]);

void getFullFileName(const char *dirName, const char *fileName, char *dest, int destLen);

int nextInDir(DIR *dir, const char *dirName, char *destFileName, int destLen);

int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);
size_t getFileSize(FILE *fp);

//
//
////   end header file   /////////////////////////////////////////////////////
#ifdef LIBZATAR_IMPL

/*********************************************
                 Cursor
**********************************************/
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>

static struct termios originalTermios;

Result enableRawMode(int vminKeys, int vtime)
{
	if (tcgetattr(STDIN_FILENO, &originalTermios) == -1) {
		return Err;
	}

	struct termios raw = originalTermios;
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

Result disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios) == -1) {
		return Err;
	}

	return Ok;
}

void disableLineWrap()
{
	printf("\033[?7l");
}

void enableLineWrap()
{
	printf("\033[?7h");
}

void hideCursor()
{
	printf("\033[?25l");
}

void showCursor()
{
	printf("\033[?25h");
}

void setCursorStyle(CURSOR_STYLE style)
{
	printf("\033[%d q", (int)style);
}

Result getCursorPos(int *x, int *y)
{
	printf("\033[6n");

	if (scanf("\033[%d;%dR", y, x) == 2) {
		return Ok;
	}

	return Err;
}

void setCursorPos(int x, int y)
{
	printf("\033[%d;%dH", y ,x);
}

void setCursorX(int x)
{
	printf("\033[%dG", x);
}

void cursorUp(int n)
{
	printf("\033[%dA", n);
}

void cursorDown(int n)
{
	printf("\033[%dB", n);
}

void cursorRight(int n)
{
	printf("\033[%dC", n);
}

void cursorLeft(int n)
{
	printf("\033[%dD", n);
}

void saveCursorPos()
{
	printf("\033[s");
}

void restoreCursorPos()
{
	printf("\033[u");
}

void enterAlternativeScreen()
{
	printf("\033[?1049h");
}

void exitAlternativeScreen()
{
	printf("\033[?1049l");
}

void clearLine()
{
	printf("\033[K");
}

void clearScreen()
{
	printf("\033[2J");
}

void updateScreen()
{
	fflush(stdout);
}

Result getScreenSizeByCursor(int *width, int *height)
{
	setCursorPos(999, 999);

	return getCursorPos(width, height);
}

Result getScreenSizeByIoctl(int *width, int *height)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0) {
		return Err;
	}

	*width = ws.ws_col;
	*height = ws.ws_row;

	return Ok;
}

Result getScreenSize(int *width, int *height)
{
	if (getScreenSizeByIoctl(width, height) == Ok) {
		return Ok;
	}

	return getScreenSizeByCursor(width, height);
}

Result registerChangeInWindowSize(void (*funciton)(int))
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

Result enableFullBuffering(FILE *fp)
{
	// do not flush on '\n'
	if (setvbuf(fp, NULL, _IOFBF, BUFSIZ) != 0) {
		return Err;
	}

	return Ok;
}

int waitForByte()
{
	char c;

	if (read(STDIN_FILENO, &c, 1) != 1) {
		return EMPTY_KEY;
	}

	return c;
}

#define SEQUENCE(a, b) ((unsigned short)((((unsigned short)b) << 8) | a))

int readEscapeKey()
{
	unsigned short key;

	if (read(STDIN_FILENO, &key, 2) != 2) {
		return '\033';
	}


	switch (key) {
		case SEQUENCE('[', 'A'): return ARROW_UP;
		case SEQUENCE('[', 'B'): return ARROW_DOWN;
		case SEQUENCE('[', 'C'): return ARROW_RIGHT;
		case SEQUENCE('[', 'D'): return ARROW_LEFT;
		case SEQUENCE('[', '1'): return HOME; // might be with a ~
		case SEQUENCE('[', '5'): return PAGE_UP; // might be with a ~
		case SEQUENCE('[', '6'): return PAGE_DOWN; // might be with a ~
	}

	return '\033';
}

int readKey()
{
	char c = waitForByte();

	if (c == '\033') {
		return readEscapeKey();
	}

	return c;
}

/*********************************************
            Path IMPLEMENTATION
**********************************************/

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

const char *getPathExtention(const char *path)
{
	const char *lastDot = strrchr(path, '.');

	if (lastDot == NULL) {
		return path;
	}

	return lastDot + 1;
}

const char *getHomePath()
{
	const char *home = getenv("HOME");

	if (home == NULL) {
		return ".";
	}

	return home;
}

char *expandPath(const char *path)
{
	if (path[0] == '~') {

		const char *home = getHomePath();
		int size = getFmtSize("%s%s", home, path + 1) + 1;
		char *dst = malloc(sizeof(char) * size);
		snprintf(dst, size, "%s%s", home, path + 1);

		return dst;
	}

	return strdup(path);
}

void compressPath(char *path)
{
	const char *home = getHomePath();
	const int homeLen = strlen(home);

	if (strncmp(home, path, homeLen) == 0) {
		const int bufLen = strlen(path) + homeLen;
		char buf[bufLen];
		snprintf(buf, bufLen, "~%s", path + homeLen);
		strncpy(path, buf, bufLen);
	}
}

Result nextInDir(DIR *dir, const char *dirName, char *destFileName, const int destLen)
{
	struct dirent *de = readdir(dir);

	if (de == NULL) {
		return Err;
	}

	snprintf(destFileName, destLen, "%s/%s", dirName, de->d_name);

	return Ok;
}

Result dirTraverse(const char *dir, bool (*action)(const char *))
{
	struct dirent *de;
	DIR *dr = opendir(dir);

	if (dr == NULL) {
		return Err;
	}

	while ((de = readdir(dr)) != NULL) {

		const char *file = de->d_name;
		char fullPath[PATH_MAX];

		const int len = snprintf(fullPath, PATH_MAX, "%s/%s", dir, file);
		fullPath[len] = '\0';

		if (action(fullPath) == false) {
			break;
		}
	}

	closedir(dr);

	return Ok;
}

bool isExtentionEqual(const char *path, const char *extention)
{
	return strcmp(getPathExtention(path), extention) == 0;
}

bool isDir(const char *path)
{
	struct stat sb;
	stat(path, &sb);

	return S_ISDIR(sb.st_mode);
}

bool isRegularFile(const char *path)
{
	struct stat sb;
	stat(path, &sb);

	return S_ISREG(sb.st_mode);
}

bool isPathExists(const char *path)
{
	return !access(path, F_OK);
}

Result echoFileWrite(const char *fileName, const char *fmt, ...)
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

Result echoFileAppend(const char *fileName, const char *fmt, ...)
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

Result readFile(const char *fileName, const char *fmt, ...)
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

Result redirectFd(int srcFd, const char *destFileName)
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

Result traverseFile(const char *fileName, const int bufSize, bool (*action)(char[bufSize]))
{
	FILE *fp = fopen(fileName, "r");

	if (fp == NULL) {
		return Err;
	}

	char buf[bufSize];
	while (fgets(buf, bufSize, fp) && action(buf));

	return Ok;
}

void getFullFileName(const char *dirName, const char *fileName, char *dest, int destLen)
{
	snprintf(dest, destLen, "%s/%s", dirName, fileName);
}

Result popen2(char *path, char *argv[], FILE *ppipe[2])
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
		execvp(path, argv);
		exit(EXIT_FAILURE);
	}

	return Ok;
}

/*********************************************
                String IMPLEMENTATION
**********************************************/

size_t getFileSize(FILE *fp)
{
	const size_t curr = ftell(fp);
	fseek(fp, 0, SEEK_END);

	const size_t size = ftell(fp);
	fseek(fp, curr, SEEK_SET);

	return size;
}

int getFmtSize(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	const int size = getFmtSizeVa(fmt, ap);
	va_end(ap);

	return size;
}

int getFmtSizeVa(const char *fmt, va_list ap)
{
	va_list ap1;
	va_copy(ap1, ap);

	const int size = vsnprintf(NULL, 0, fmt, ap1);
	va_end(ap1);

	return size;
}


#endif // LIBZATAR_IMPL
#endif // LIBZATRAR_h
