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

HWND makeWindow();

int findWidthOfWindow();

int findHeightOfWindow();

void createField(int size_x, int size_y);

void clearWindow();

#endif //TESTWINDOW_SETFIELD_H
