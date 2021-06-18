#ifndef TICTACTOE_CLIENTGRAPHICS_H
#define TICTACTOE_CLIENTGRAPHICS_H

#include "../SetField/SetField.h"
#include "../gameboard/gameboard.h"
#include "../playerlist/playerlist.h"
#include <windows.h>

typedef struct menu_struct
{
    int menuId;
    int numSubmenus;
    int isPlayerlist;
    char menuTitle[32];
    char submenu1[32];
    int submenu1Id;
    char submenu2[32];
    int submenu2Id;
    char submenu3[32];
    int submenu3Id;
}MENU;

void drawMenu(HANDLE window, MENU *targetMenu, int currentSubmenu, int x, int y);

void draw3x3BoardAt(HANDLE window, GAME_BOARD* targetBoard, int selectedRow, int selectedColumn,int x, int y);

void drawPlayerlistAt(HANDLE window, PLAYERLIST* targetPlayerlist, int x, int y, int maxAmount);

#endif //TICTACTOE_CLIENTGRAPHICS_H
