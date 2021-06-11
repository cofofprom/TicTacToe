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

int findWidthOfWindow()                             //Найдём ширину окна (сколько символов помещается по горизонтали)
{
    HWND hWndConsole;                               //Дескриптор окна
    int Width = 0;

    if (hWndConsole = GetStdHandle(STD_ERROR_HANDLE))//возвращает 1 если дескиптор получен без ошибок, иначе 0
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
            Width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
        else printf("Error: %d\n", GetLastError());
    }
    else printf("Error: %d\n", GetLastError());
    return Width;
}

int findHeightOfWindow()                            //Найдём высоту окна (сколько символов помещается по вертикали)
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

void alignField(int width, int height)
{
    int WidthOfCell = min(width / 3, (height - 1) / 3);
    for (int SpaceIndx = 0; SpaceIndx < (width - WidthOfCell * 3 * 2) / 2; SpaceIndx++)
    {
        printf(" ");
    }
}

void createField(int size_x, int size_y)            //Создание поля во всё окно
{
    int Divisor_x = size_x / 3, Divisor_y = (size_y - 1) / 3, CommonDivisor = min(Divisor_x, Divisor_y);
    alignField(size_x, size_y);
    for (int y_coord = 0; y_coord <= CommonDivisor * 3; y_coord++)
    {
        for (int x_coord = 0; x_coord < CommonDivisor * 3; x_coord++)
        {
            if (x_coord == CommonDivisor * 3 - 1)
            {
                if (y_coord == 0 || y_coord == CommonDivisor * 3)
                {
                    printf("\n");
                    alignField(size_x, size_y);
                }
                else
                {
                    printf("|\n");
                    alignField(size_x, size_y);
                }
            }
            else if ((x_coord == 0 || x_coord == CommonDivisor * 3 - 1) && y_coord != 0 &&
                     y_coord != CommonDivisor * 3)
                printf("|");
            else if ((y_coord == 0 || y_coord == CommonDivisor * 3) && x_coord != 0 &&
                     x_coord != CommonDivisor * 3 - 1)
                printf("--");
            else if (y_coord % CommonDivisor == 0) printf("--");
            else if (x_coord % CommonDivisor == 0) printf("| ");
            else printf("  ");
        }
    }
}

void clearWindow()
{
    system("cls");
}

void printStrAtConsolePos(HANDLE hwd, short x, short y, char* str)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hwd, pos);
    WriteConsole(hwd, str, strlen(str), 0, 0);
}