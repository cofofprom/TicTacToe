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

#define bool int
#define true 1
#define false 0

#define LEFT_UPPER_ANGLE_OF_FIELD_X 25
#define LEFT_UPPER_ANGLE_OF_FIELD_Y 0

#define BLACK_ON_WHITE 240
#define FULL_WHITE 255

HWND makeWindow();

int findWidthOfWindow();

int findHeightOfWindow();

void printStrAtConsolePos(HANDLE hwd, int x, int y, char* str, WORD attr);

void createField(HWND ConsoleHandle, int size_x, int size_y);

void clearWindow(HWND ConsoleHandle);

void moveCursor(HWND hWnd, int x, int y);

void showConsoleCursor(bool showFlag);

void drawCross(HWND ConsoleHandle, int size_x, int size_y, int x, int y);

void drawCircle(HWND ConsoleHandle, int size_x, int size_y, int x, int y);

int newMotion(int Turn, HWND ConsoleHandle, int size_x, int size_y, int x, int y);

#endif //TESTWINDOW_SETFIELD_H
