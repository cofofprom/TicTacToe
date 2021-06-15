#include <stdio.h>
#include <stdlib.h>
#include "..\SetField\SetField.h"

int main(int argc,char** argv)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    printStrAtConsolePos(console, 1, 1, "t", FOREGROUND_RED);
    printStrAtConsolePos(console, 15, 1, "t2", BACKGROUND_GREEN);
    return 0;
}