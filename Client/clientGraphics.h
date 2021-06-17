#ifndef TICTACTOE_CLIENTGRAPHICS_H
#define TICTACTOE_CLIENTGRAPHICS_H

typedef struct menu_struct
{
    int menuId;
    char* menuName;
    int numSubmenus;
    struct menu_struct* submenus;
}MENU;

MENU* makeMenu(int menuId, char* menuName);

void freeMenu(MENU* targetMenu);

void addSubmenu(MENU* targetMenu, MENU* addedSubmenu);

void printMenu(MENU* targetMenu);

#endif //TICTACTOE_CLIENTGRAPHICS_H
