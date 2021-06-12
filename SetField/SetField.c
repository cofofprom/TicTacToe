#include "SetField.h"

HWND makeWindow()
{
    HWND hwnd;                                                                  //Дескриптор окна
    char Title[] = "TicTacToe";
    SetConsoleTitle(Title);                                                     // Устанваливаем заголовок окна консоли
    hwnd = GetStdHandle(STD_OUTPUT_HANDLE);                                     // Находим дескриптор окна

//    MoveWindow(hwnd, WINDOW_SHIFT_X, WINDOW_SHIFT_Y, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);//Устанавливаем размеры и смещение окна

    return hwnd;                                                                //Возвращаем дескриптор окна
}

int findWidthOfWindow()                                                         //Найдём ширину окна (сколько символов помещается по горизонтали)
{
    HWND hWndConsole;                                                           //Дескриптор окна
    int Width = 0;
    if (hWndConsole = GetStdHandle(STD_ERROR_HANDLE))                           //возвращает 1 если дескиптор получен без ошибок, иначе 0
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
            Width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
        else printf("Error: %d\n", GetLastError());
    }
    else printf("Error: %d\n", GetLastError());
    return Width;
}

int findHeightOfWindow()                                                        //Найдём высоту окна (сколько символов помещается по вертикали)
{
    HWND hWndConsole;
    int Height = 0;
    if (hWndConsole = GetStdHandle(-12))
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
            Height = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
        else printf("Error: %d\n", GetLastError());
    }
    else printf("Error: %d\n", GetLastError());
    return Height;
}

void printStrAtConsolePos(HANDLE hwd, int x, int y, char *str, WORD attr)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hwd, pos);
    SetConsoleTextAttribute(hwd, attr);
    WriteConsole(hwd, str, strlen(str), 0, 0);
    //SetConsoleTextAttribute(hwd, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN); если надо оставь, с этом оно не будет высирать следущие символы с таким же аттрибутом
}

//createField рисует игровое поле по середине консоли и делает клетки поля квадратными, длины сторон зависят от размеров окна
void createField(HWND ConsoleHandle, int size_x, int size_y)
{
    int Divisor_x = size_x / 3, Divisor_y = (size_y - 1) / 3, CommonDivisor = min(Divisor_x, Divisor_y) + Divisor_x / 3;
    char *FillLine = (char *) calloc(Divisor_x * 3 + 1, 1);
    for (int i = 0; i < CommonDivisor * 3; i++) FillLine[i] = '_';
    printStrAtConsolePos(ConsoleHandle, LEFT_UPPER_ANGLE_OF_FIELD_X, LEFT_UPPER_ANGLE_OF_FIELD_Y, FillLine, BLACK_ON_WHITE);
    printStrAtConsolePos(ConsoleHandle, LEFT_UPPER_ANGLE_OF_FIELD_X, LEFT_UPPER_ANGLE_OF_FIELD_Y + Divisor_y, FillLine, BLACK_ON_WHITE);
    printStrAtConsolePos(ConsoleHandle, LEFT_UPPER_ANGLE_OF_FIELD_X, LEFT_UPPER_ANGLE_OF_FIELD_Y + Divisor_y * 2, FillLine, BLACK_ON_WHITE);
    printStrAtConsolePos(ConsoleHandle, LEFT_UPPER_ANGLE_OF_FIELD_X, LEFT_UPPER_ANGLE_OF_FIELD_Y + Divisor_y * 3, FillLine, BLACK_ON_WHITE);
    for (int cnt = 0; cnt <= 3; cnt++)
    {
        int current_x = LEFT_UPPER_ANGLE_OF_FIELD_X + cnt * CommonDivisor;
        for (int i = 0; i <= LEFT_UPPER_ANGLE_OF_FIELD_Y + Divisor_y * 3; i++)
        {
            if (i != 0)
            {
                printStrAtConsolePos(ConsoleHandle, current_x, LEFT_UPPER_ANGLE_OF_FIELD_Y + i, "|", BLACK_ON_WHITE);
            }
        }
    }
    printStrAtConsolePos(ConsoleHandle, LEFT_UPPER_ANGLE_OF_FIELD_X, LEFT_UPPER_ANGLE_OF_FIELD_Y, " ", FULL_WHITE);
}

//Очищает консоль, закрашивая её полностью в фоновый цвет
void clearWindow(HWND ConsoleHandle)
{
    int WidthOfWindow = findWidthOfWindow(), HeightOfWindow = findHeightOfWindow();
    char *FillInBreadth = (char *) calloc(WidthOfWindow + 1, 1);
    for (int i = 0; i < WidthOfWindow; i++) FillInBreadth[i] = ' ';
    for (int i = 0; i < HeightOfWindow; i++)
    {
        printStrAtConsolePos(ConsoleHandle, 0, i, FillInBreadth, FULL_WHITE);
    }
    moveCursor(ConsoleHandle, 0, 0);
    showConsoleCursor(0);
}

//Передвигает курсор в указанные координаты. Нужно для того, чтобы после отрисовки объектов
//консоль не скролилась вниз (после каждой операции курсор смещаем в (0, 0)).
void moveCursor(HWND ConsoleHandle, int x, int y)
{
    COORD Position;
    Position.X = x;
    Position.Y = y;
    SetConsoleCursorPosition(ConsoleHandle, Position);
}

//Убирает курсор при необходимости. Нужно, например, для того, чтобы он не мигал после отривоки игрового поля
void showConsoleCursor(bool showFlag)
{
    HWND Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(Handle, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(Handle, &cursorInfo);
}

//Рисует крестик в относительных координатах 1 <= x, y <= 3 при помощи ascii символов
void drawCross(HWND ConsoleHandle, int size_x, int size_y, int x, int y)
{
    int Divisor_x = size_x / 3, Divisor_y = (size_y - 1) / 3 + 2, CommonDivisor = min(Divisor_x, Divisor_y) + Divisor_x / 3;
    int current_x = LEFT_UPPER_ANGLE_OF_FIELD_X + CommonDivisor / 2 + CommonDivisor * (x - 1) - (3 + x + (x - 1) * 0.5), current_y =
            LEFT_UPPER_ANGLE_OF_FIELD_Y + Divisor_y / 2 + Divisor_y * (y - 1) - (y);
    if (y < 2) current_y += Divisor_y / 6;
    if (y > 2) current_y -= Divisor_y / 6;
    printStrAtConsolePos(ConsoleHandle, current_x, current_y - 3, "\\     /", BLACK_ON_WHITE);
    printStrAtConsolePos(ConsoleHandle, current_x, current_y - 2, " \\   / ", BLACK_ON_WHITE);
    printStrAtConsolePos(ConsoleHandle, current_x, current_y - 1, "  \\ /  ", BLACK_ON_WHITE);
    printStrAtConsolePos(ConsoleHandle, current_x, current_y, "   X   ", BLACK_ON_WHITE);
    printStrAtConsolePos(ConsoleHandle, current_x, current_y + 1, "  / \\  ", BLACK_ON_WHITE);
    printStrAtConsolePos(ConsoleHandle, current_x, current_y + 2, " /   \\ ", BLACK_ON_WHITE);
    printStrAtConsolePos(ConsoleHandle, current_x, current_y + 3, "/     \\", BLACK_ON_WHITE);
}

//Рисует нолик в относительных координатах 1 <= x, y <= 3 при помощи ascii символов
void drawCircle(HWND ConsoleHandle, int size_x, int size_y, int x, int y)
{
    //Это Ваня сделает
}

int newMotion(int Turn, HWND ConsoleHandle, int size_x, int size_y, int x, int y)
{
    //Рисуем крестик или нолик в зависимости от того, чья сейчас очередь
}


