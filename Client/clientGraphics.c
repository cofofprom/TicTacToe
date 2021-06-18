#include "clientGraphics.h"

void drawMenu(HANDLE window, MENU *targetMenu, int currentSubmenu, int x, int y) {
    printStrAtConsolePos(window,x,y,&targetMenu->menuTitle,WHITE_ON_BLACK);
    if(targetMenu->submenu1Id != -1)
    {
        if(currentSubmenu == 1)
        {
            printStrAtConsolePos(window,x+1,y+1,&targetMenu->submenu1,BLACK_ON_WHITE);
        }
        else
        {
            printStrAtConsolePos(window,x+1,y+1,&targetMenu->submenu1,WHITE_ON_BLACK);
        }
    }
    if(targetMenu->submenu2Id != -1)
    {
        if(currentSubmenu == 2)
        {
            printStrAtConsolePos(window,x+1,y+2,&targetMenu->submenu2,BLACK_ON_WHITE);
        }
        else
        {
            printStrAtConsolePos(window,x+1,y+2,&targetMenu->submenu2,WHITE_ON_BLACK);
        }
    }
    if(targetMenu->submenu3Id != -1)
    {
        if(currentSubmenu == 3)
        {
            printStrAtConsolePos(window,x+1,y+3,&targetMenu->submenu3,BLACK_ON_WHITE);
        }
        else
        {
            printStrAtConsolePos(window,x+1,y+3,&targetMenu->submenu3,WHITE_ON_BLACK);
        }
    }
}

void draw3x3BoardAt(HANDLE window, GAME_BOARD *targetBoard, int selectedRow, int selectedColumn, int x, int y) {

    char firstRow[] = {201,205,203,205,203,205,187,0};
    char lastRow[] = {200,205,202,205,202,205,188,0};
    char divRow[] = {204,205,206,205,206,205,185,0};
    char typicalRow[] = {204,'a',206,'a',206,'a',185,0};
    char rowStart[] = {204,0};
    char rowEnd[] = {185,0};
    char rowMid[] = {206,0};
    char cellRow[] = {'a',0};

    system("cls");

    for(int i = 0; i < 7; i++)
    {
        if(i == 0)
        {
            printStrAtConsolePos(window,x,y,firstRow,WHITE_ON_BLACK);
            continue;
        }
        if(i == 6)
        {
            printStrAtConsolePos(window,x,y+i,lastRow,WHITE_ON_BLACK);
            continue;
        }
        if(i%2 == 0)
        {
            printStrAtConsolePos(window,x,y+i,divRow,WHITE_ON_BLACK);
            continue;
        }

        for(int j = 0; j < 7; j ++)
        {
            if(j == 0)
            {
                printStrAtConsolePos(window,x+j,y+i,rowStart,WHITE_ON_BLACK);
                continue;
            }
            if(j == 6)
            {
                printStrAtConsolePos(window,x+j,y+i,rowEnd,WHITE_ON_BLACK);
                continue;
            }
            if(j%2 == 0)
            {
                printStrAtConsolePos(window,x+j,y+i,rowMid,WHITE_ON_BLACK);
                continue;
            }

            CELL_TYPE cell = getCellTypeAt(targetBoard,i/2,j/2);

            switch(cell)
            {
                case ZeroCell:
                    cellRow[0] = 'O';
                    break;

                case CrossCell:
                    cellRow[0] = 'X';
                    break;

                case EmptyCell:
                    cellRow[0] = ' ';
                    break;
            }

            if(selectedColumn == j/2 && selectedRow == i/2)
            {
                printStrAtConsolePos(window,x+j,y+i,cellRow,BLACK_ON_WHITE);
                continue;
            }else
            {
                printStrAtConsolePos(window,x+j,y+i,cellRow,WHITE_ON_BLACK);
                continue;
            }
        }
    }
}

void drawPlayerlistAt(HANDLE window, PLAYERLIST* targetPlayerlist, int x, int y, int maxAmount) {
    char playerlistTitle[] = "Online players:";
    printStrAtConsolePos(window,x,y,playerlistTitle,WHITE_ON_BLACK);
    int iterlim = min(maxAmount,targetPlayerlist->playerListLength);
    for(int i = 0; i < iterlim; i++)
    {
        printStrAtConsolePos(window,x+1,y+i,targetPlayerlist->playerdataList[i]->username,WHITE_ON_BLACK);
    }
}
