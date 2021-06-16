#include <stdio.h>
#include <stdlib.h>
#include "..\SetField\SetField.h"

int tipoArtemsCallback(int turn, int x, int y)
{
    // делает с данными что хочет
    return 0;
}

int main()
{
    HWND StdHandle = makeWindow(), ConsoleHandle = GetConsoleWindow();
    startGame(StdHandle, ConsoleHandle, tipoArtemsCallback);
}