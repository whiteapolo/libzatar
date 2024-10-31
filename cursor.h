#ifndef CURSOR_H
#define CURSOR_H

#ifndef RESULT
#define RESULT
typedef enum { Ok = 0, Err = -1, } Result;
#endif

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

#ifdef CURSOR_IMPL
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <signal.h>
#include "error.h"

static struct termios originalTermios;

Result enableRawMode(int vminKeys, int vtime)
{
	if (tcgetattr(STDIN_FILENO, &originalTermios) == -1)
		return Err;

	struct termios raw = originalTermios;
	raw.c_cc[VMIN] = vminKeys;
	raw.c_cc[VTIME] = vtime;
	raw.c_cflag |= (CS8);
	raw.c_oflag &= ~(OPOST);
	raw.c_iflag &= ~(IXON | ICRNL | ISTRIP | INPCK | BRKINT);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		return Err;

	return Ok;
}

Result disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios) == -1)
		return Err;
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
	if (scanf("\033[%d;%dR", y, x) == 2)
		return Ok;
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

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0)
		return Err;

	*width = ws.ws_col;
	*height = ws.ws_row;
	return Ok;
}

Result getScreenSize(int *width, int *height)
{
	if (getScreenSizeByIoctl(width, height) == Ok)
		return Ok;

	return getScreenSizeByCursor(width, height);
}

Result registerChangeInWindowSize(void (*funciton)(int))
{
	struct sigaction sa;
	sa.sa_handler = funciton;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGWINCH, &sa, NULL) == -1)
		return Err;

	return Ok;
}

Result enableFullBuffering(FILE *fp)
{
	// do not flush on '\n'
	if (setvbuf(fp, NULL, _IOFBF, BUFSIZ) != 0)
		return Err;
	return Ok;
}

int waitForByte()
{
	char c;
	if (read(STDIN_FILENO, &c, 1) != 1)
		return EMPTY_KEY;
	return c;
}

#define SEQUENCE(a, b) ((unsigned short)((((unsigned short)b) << 8) | a))

int readEscapeKey()
{
	unsigned short key;

	if (read(STDIN_FILENO, &key, 2) != 2)
		return '\033';

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
	if (c == '\033')
		return readEscapeKey();
	return c;
}

#endif

#endif
