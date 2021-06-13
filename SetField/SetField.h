#ifndef TICTACTOE_SETFIELD_H
#define TICTACTOE_SETFIELD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
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
#define FULL_BLACK 0

HWND makeWindow();                                                            //Инициализация окна, получение дескриптора

int findWidthOfWindow();                                                      //Ширина окна

int findHeightOfWindow();                                                     //Высота окна

void printStrAtConsolePos(HANDLE hwd, int x, int y, char* str, WORD attr);    //Печать указателя на строку, начиная с определённых координат (x, y)

void createField(HWND StdHandle, int size_x, int size_y);                     //Вывод игрового поля 3x3

void clearWindow(HWND StdHandle);                                             //Очистка экрана

void moveCursor(HWND hWnd, int x, int y);                                     //Перемещение курсора в указанные координаты (x, y)

void showConsoleCursor(bool showFlag);                                        //Видимость курсора в консоли (1 - виден, 0 - не виден)

void drawCross(HWND StdHandle, int size_x, int size_y, int x, int y);         //Рисование крестика на поле в относительных координатах 1 <= x, y <= 3

void drawCircle(HWND StdHandle, int size_x, int size_y, int x, int y);        //Рисование нолика на поле в относительных координатах 1 <= x, y <= 3

int newMotion(int Turn, HWND WindowStdInputHandle, HWND ConsoleHandle, int size_x, int size_y, int x, int y);//Рисует крестик или нолик в зависимости от того, кто сейчас ходит

COORD getTextCursorCoords();

void startGame(HWND WindowStdInputHandle, HWND ConsoleHandle);                //Начинаем игру

void testMouseClick(HWND WindowStdInputHandle);

void resetConsole(HWND StdHandle);

void checkAllFigures();

#endif //TICTACTOE_SETFIELD_H
