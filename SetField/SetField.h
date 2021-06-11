#ifndef TESTWINDOW_SETFIELD_H
#define TESTWINDOW_SETFIELD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define WINDOW_SHIFT_X 550
#define WINDOW_SHIFT_Y 250
#define WINDOW_WIDTH   200
#define WINDOW_HEIGHT  200
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

HWND makeWindow();

int findWidthOfWindow();

int findHeightOfWindow();

void alignField(int width, int height);

void createField(int size_x, int size_y);

void clearWindow();

/* function prints str at the specified (x, y) pos of the console hwd with color
 * color is combination of FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE or
 * BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE
 * Example: FOREGROUND_RED | FOREGROUND_BLUE stands for purple
*/
void printStrAtConsolePos(HANDLE hwd, short x, short y, char* str, WORD color);

#endif //TESTWINDOW_SETFIELD_H
