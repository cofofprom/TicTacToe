#ifndef TICTACTOE_SETFIELD_H
#define TICTACTOE_SETFIELD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>

#define WINDOW_SHIFT_X 50
#define WINDOW_SHIFT_Y 50
#define WINDOW_WIDTH   120
#define WINDOW_HEIGHT  30

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define bool    int
#define true    1
#define false   0

//Левый верхний угол игрового поля в символьных координатах
#define LEFT_UPPER_ANGLE_OF_FIELD_X 25
#define LEFT_UPPER_ANGLE_OF_FIELD_Y 0

//Границы клеток игрового поля в координатах курсора мыши
#define TOP_1Y      15
#define BOTTOM_1Y   158
#define TOP_2Y      159
#define BOTTOM_2Y   302
#define TOP_3Y      303
#define BOTTOM_3Y   477
#define LEFT_1X     183
#define RIGHT_1X    340
#define LEFT_2X     341
#define RIGHT_2X    498
#define LEFT_3X     499
#define RIGHT_3X    658

//Аттрибуты для вывода текста с разными цветами и фоном
#define BLACK_ON_WHITE  BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED
#define WHITE_ON_BLACK  FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN
#define FULL_WHITE      255
#define FULL_BLACK      0

HWND makeWindow();                                                            //Инициализация окна, получение дескриптора

int findWidthOfWindow();                                                      //Ширина окна

int findHeightOfWindow();                                                     //Высота окна

void printStrAtConsolePos(HANDLE hwd, int x, int y, char* str, WORD attr);    //Печать указателя на строку, начиная с определённых координат (x, y)

void createField(HWND StdHandle, int size_x, int size_y, int Color);          //Вывод игрового поля 3x3

void clearWindow(HWND StdHandle);                                             //Очистка экрана

void moveCursor(HWND hWnd, int x, int y);                                     //Перемещение курсора в указанные координаты (x, y)

void showConsoleCursor(bool showFlag);                                        //Видимость курсора в консоли (1 - виден, 0 - не виден)

void drawCross(HWND StdHandle, int size_x, int size_y, int x, int y);         //Рисование крестика на поле в относительных координатах 1 <= x, y <= 3

void drawCircle(HWND StdHandle, int size_x, int size_y, int x, int y);        //Рисование нолика на поле в относительных координатах 1 <= x, y <= 3

int newMotion(int Turn, HWND WindowStdInputHandle, HWND ConsoleHandle, int size_x, int size_y, int x, int y);//Рисует крестик или нолик в зависимости от того, кто сейчас ходит

COORD getTextCursorCoords();                                                  //Возвращает координаты текстового курсора (в структуре COORD)

void startGame(HWND WindowStdInputHandle, HWND ConsoleHandle, int (*networkingCallback)(int, int, int));                //Начинаем игру

void testMouseClick(HWND WindowStdInputHandle);                               //Корректность считывания координат нажатия ЛКМ

void resetConsole(HWND StdHandle);                                            //Возврат консоли в состояние по умолчанию

void checkAllFigures();                                                       //Последовательный вывод крестиков и ноликов в каждой клетке

#endif //TICTACTOE_SETFIELD_H
