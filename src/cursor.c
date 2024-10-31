#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>
#include "cursor.h"

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
