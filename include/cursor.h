#ifndef CURSOR_H
#define CURSOR_H
#include "shared.h"

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

#endif
