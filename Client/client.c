//#include <stdio.h>
//#include <stdlib.h>
//#include "..\SetField\SetField.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../clientNetworking/clientNetworkWorker.h"
#include "../clientNetworking/packet.h"
#include "../clientNetworking/packetEnums.h"
#include "../clientNetworking/packetQueue.h"
#include "../gameboard/gameboard.h"
#include "../SetField/SetField.h"
#include "../playerlist/playerlist.h"
#include "clientGraphics.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
HANDLE hStdin;
DWORD fdwSaveOldMode;

#define KEYCODE_UPARROW 38
#define KEYCODE_LEFTARROW 37
#define KEYCODE_DOWNARROW 40
#define KEYCODE_RIGHTARROW 39
#define KEYCODE_ENTER 13
#define KEYCODE_ESCAPE 27

#define CHAR_SCR_SIZE 25

VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);

int main(int argc, char** argv)
{
    DWORD cNumRead, fdwMode, i;
    INPUT_RECORD irInBuf[128];
    MENU mainMenu = {0};
    mainMenu.menuId = 0;
    strcpy(&mainMenu.menuTitle, "Main menu");
    strcpy(&mainMenu.submenu1, "Send game request");
    strcpy(&mainMenu.submenu2, "Online players");
    strcpy(&mainMenu.submenu3, "Incoming invites");
    mainMenu.submenu1Id = 1;
    mainMenu.submenu2Id = 2;
    mainMenu.submenu3Id = 3;
    mainMenu.numSubmenus = 3;

    GAME_BOARD* currentBoard = NULL;
    PLAYERLIST* currentPlayerlist = NULL;
    int currentBoardRow = 0;
    int currentBoardColumn = 0;
    int inGame = 0;
    int boardRedraw = 0;
    int awaitingMove = 0;
    int endgameWait = 0;
    int viewingPlayerList = 0;

    MENU* menus = calloc(10,sizeof(MENU));
    menus[mainMenu.menuId] = mainMenu;

    int currentMenuId = 0;
    int currentSubmenuIndex = 1;
    int DBGDisableLogin = 1;
    int workerErr = 0;
    NETWORK_WORKER* clientWorker = initNewNetworkWorker(workerMain,"127.0.0.1",5510,&workerErr);
    char Title[] = "Tic Tac Toe";
    char* password = calloc(33,sizeof(char));
    char* login = calloc(33,sizeof(char));
    char* requestedGameNickname = calloc(33,sizeof(char));
    char test[] = "Test\0";
    CELL_TYPE myCellType = -1;
    CELL_TYPE oppCellType = -1;
    KEY_EVENT_RECORD ker;

    HANDLE consoleScr = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitle(Title);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowSize = min(screenWidth,screenHeight) / 2;
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    MoveWindow(consoleWindow, WINDOW_SHIFT_X,WINDOW_SHIFT_Y,windowSize,windowSize,TRUE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    GetCurrentConsoleFontEx(consoleScr,FALSE,&cfi);
    cfi.dwFontSize.X = windowSize / CHAR_SCR_SIZE;
    cfi.dwFontSize.Y = windowSize / CHAR_SCR_SIZE;
    SetCurrentConsoleFontEx(consoleScr,FALSE,&cfi);
    COORD bufferSize = {CHAR_SCR_SIZE, CHAR_SCR_SIZE};
    SetConsoleScreenBufferSize(consoleScr, bufferSize);
    //ShowScrollBar(consoleWindow,SB_BOTH,0);
    //EnableScrollBar(consoleWindow,SB_BOTH,ESB_DISABLE_BOTH);
    //while(clientWorker->receivedPacketQueue->length == 0){}
    PACKET* lastPacket = getPacketFromClientWorker(clientWorker,&workerErr);

    do
    {
        freePacket(lastPacket);
        printf("Enter login:");
        fflush(stdin);
        fgets(login,32,stdin);

        printf("Enter password:");
        fflush(stdin);
        fgets(password,32,stdin);

        login[strlen(login)-1] = 0;
        password[strlen(password)-1] = 0;

        sendLoginData(clientWorker,login,password);

        while(clientWorker->receivedPacketQueue->length == 0){}
        lastPacket = getPacketFromClientWorker(clientWorker,&workerErr);
        system("cls");
    } while (lastPacket->packetType != ServicePacket &&
    lastPacket->packetSubtype != ServiceSuccess);
    freePacket(lastPacket);

    printf("Successfully logged in! Loading...\n");
    system("cls");

    drawMenu(consoleScr,&mainMenu,currentSubmenuIndex,2,2);

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");

    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    fdwMode = fdwMode & (~ENABLE_ECHO_INPUT);
    if (! SetConsoleMode(hStdin, fdwMode) )
        ErrorExit("SetConsoleMode");

    //Main loop
    while (1)
    {
        int numEvents = 0;
        GetNumberOfConsoleInputEvents(hStdin,&numEvents);
        if(numEvents > 0) {
            if (!ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead)) ErrorExit("ReadConsoleInput");

            // Dispatch the events to the appropriate handler.
            for (i = 0; i < cNumRead; i++) {
                switch (irInBuf[i].EventType) {
                    case KEY_EVENT: // keyboard input
                        ker = irInBuf[i].Event.KeyEvent;
                        //KeyEventProc(irInBuf[i].Event.KeyEvent);
                        if (ker.bKeyDown) {
                            switch (ker.wVirtualKeyCode) {
                                case KEYCODE_ESCAPE:
                                    if (currentMenuId == mainMenu.menuId) {
                                        exit(0);
                                    } else {
                                        currentMenuId = mainMenu.menuId;
                                        drawMenu(consoleScr, &mainMenu, currentSubmenuIndex, 2, 2);
                                    }
                                    break;

                                case KEYCODE_UPARROW:
                                    if(endgameWait) break;
                                    if (inGame) {
                                        currentBoardRow--;
                                        boardRedraw = 1;
                                        if (currentBoardRow < 0) {
                                            currentBoardRow = 2;
                                        }
                                    } else {
                                        currentSubmenuIndex--;
                                        if (currentSubmenuIndex < 1) {
                                            currentSubmenuIndex = menus[currentMenuId].numSubmenus;
                                        }
                                        drawMenu(consoleScr, &menus[currentMenuId], currentSubmenuIndex, 2, 2);
                                    }
                                    break;

                                case KEYCODE_DOWNARROW:
                                    if(endgameWait) break;
                                    if (inGame) {
                                        boardRedraw = 1;
                                        currentBoardRow++;
                                        if (currentBoardRow > 2) {
                                            currentBoardRow = 0;
                                        }
                                    } else {
                                        currentSubmenuIndex++;
                                        if (currentSubmenuIndex > menus[currentMenuId].numSubmenus) {
                                            currentSubmenuIndex = 1;
                                        }
                                        drawMenu(consoleScr, &menus[currentMenuId], currentSubmenuIndex, 2, 2);
                                    }
                                    break;

                                case KEYCODE_ENTER:
                                    if(endgameWait)
                                    {
                                        endgameWait = 0;
                                        inGame = 0;
                                        awaitingMove = 0;
                                    }
                                    if (inGame) {
                                        if(awaitingMove && getCellTypeAt(currentBoard,currentBoardRow,currentBoardColumn) == EmptyCell) {
                                            boardRedraw = 1;
                                            awaitingMove = 0;
                                            sendBoardMove(clientWorker, currentBoardRow, currentBoardColumn);
                                            makeMove(currentBoard,currentBoardRow,currentBoardColumn,myCellType);
                                            currentBoardColumn = -1;
                                            currentBoardRow = -1;
                                        }
                                    } else {
                                        switch (currentSubmenuIndex) {
                                            case 1:
                                                do
                                                {
                                                    system("cls");
                                                    printf("Enter opponent's nickname:\n");
                                                    fflush(stdin);
                                                    fgets(requestedGameNickname, 32, stdin);
                                                    requestedGameNickname[strlen(requestedGameNickname) - 1] = 0;
                                                    if(strcmp(login,requestedGameNickname) != 0)
                                                    {
                                                        break;
                                                    }
                                                }while(1);
                                                fflush(stdin);
                                                requestGame(clientWorker, 3, requestedGameNickname);
                                                printf("Awaiting response...\n");
                                                while (clientWorker->receivedPacketQueue->length == 0) {}
                                                while(1) {
                                                    lastPacket = getPacketFromClientWorker(clientWorker, &workerErr);
                                                    //printf("Got packet");
                                                    if (lastPacket->packetType == ServicePacket &&
                                                        lastPacket->packetSubtype == ServiceErrorPacket &&
                                                        lastPacket->packetCode == GameDeclinedErr) {
                                                        printf("Game declined");
                                                        system("cls");
                                                        drawMenu(consoleScr, &mainMenu, currentSubmenuIndex, 2, 2);
                                                        free(lastPacket);
                                                        break;
                                                    } else if (lastPacket->packetSubtype == ServiceSuccess) {
                                                        printf("Game accepted\n");
                                                        system("cls");
                                                        currentBoard = initNewBoard(3);
                                                        inGame = 1;
                                                        boardRedraw = 1;
                                                        myCellType = ZeroCell;
                                                        oppCellType = CrossCell;
                                                        currentBoardColumn = -1;
                                                        currentBoardRow = -1;
                                                        free(lastPacket);
                                                        break;
                                                    }
                                                    free(lastPacket);
                                                    fflush(stdin);
                                                }
                                                break;

                                            case 2:
//                                                currentMenuId = 2;
//                                                requestFriendList(clientWorker);
//                                                while(clientWorker->receivedPacketQueue->length == 0){}
//                                                lastPacket = getPacketFromClientWorker(clientWorker,&workerErr);
//                                                currentPlayerlist = decodeFriendAndBlacklist(lastPacket->packetData);
//                                                drawPlayerlistAt(consoleScr,currentPlayerlist,1,1,25);
//                                                freePacket(lastPacket);
                                                break;
                                        }
                                    }
                                    break;

                                case KEYCODE_LEFTARROW:
                                    if(endgameWait) break;
                                    if (inGame)
                                    {
                                        boardRedraw = 1;
                                        currentBoardColumn--;
                                        if (currentBoardColumn < 0) {
                                            currentBoardColumn = 2;
                                        }
                                    }
                                    break;

                                case KEYCODE_RIGHTARROW:
                                    if(endgameWait) break;
                                    if (inGame) {
                                        boardRedraw = 1;
                                        currentBoardColumn++;
                                        if (currentBoardColumn > 2) {
                                            currentBoardColumn = 0;
                                        }
                                    }
                                    break;
                            }
                        }
                        break;

                    case MOUSE_EVENT: // mouse input
                    case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                    case FOCUS_EVENT:  // disregard focus events
                    case MENU_EVENT:   // disregard menu events
                        break;

                    default:
                        ErrorExit("Unknown event type");
                        break;
                }
            }
        }

        char* msgStr = NULL;

        //Process packets
        while(clientWorker->receivedPacketQueue->length > 0)
        {
            lastPacket = getPacketFromClientWorker(clientWorker,&workerErr);
            //printf("Got packet: %d %d %d",lastPacket->packetType,lastPacket->packetSubtype,lastPacket->packetCode);
            //if (lastPacket->packetData != NULL) printf("Packet data: %s",lastPacket->packetData);
            switch(lastPacket->packetType)
            {
                case ServicePacket:
                    switch(lastPacket->packetSubtype)
                    {
                        case ServiceUserAction:
                            switch (lastPacket->packetCode)
                            {
                                case GameRequestAction:
                                    system("cls");
                                    printf("Game request from: %s\n",lastPacket->packetData+2);
                                    char acceptStr[] = "Accept? Y/N";
                                    printStrAtConsolePos(consoleScr,2,2,acceptStr,WHITE_ON_BLACK);
                                    fflush(stdin);
                                    char ans = getchar();

                                    do {
                                        if (ans == 'Y' || ans == 'y') {
                                            acceptGameRequest(clientWorker, lastPacket->packetData + 2);
                                            currentBoard = initNewBoard(3);
                                            inGame = 1;
                                            boardRedraw = 1;
                                            awaitingMove = 1;
                                            myCellType = CrossCell;
                                            oppCellType = ZeroCell;
                                            break;
                                        } else if (ans == 'n' || ans == 'N') {
                                            declineGameRequest(clientWorker, lastPacket->packetData + 2);
                                            system("cls");
                                            drawMenu(consoleScr, &mainMenu, currentSubmenuIndex, 2, 2);
                                            break;
                                        } else {
                                            printStrAtConsolePos(consoleScr, 2, 2, acceptStr, WHITE_ON_BLACK);
                                            fflush(stdin);
                                            ans = getchar();
                                        }
                                    }
                                    while (1);

                                    break;

                                default:
                                    break;
                            }
                            break;

                        case ServiceEndGame:
                            if(inGame)
                            {
                                char reason = lastPacket->packetData[0];
                                switch(reason)
                                {
                                    case 1:
                                        system("cls");
                                        inGame = 0;
                                        awaitingMove = 0;
                                        freeGameBoard(currentBoard);
                                        msgStr = calloc(64,sizeof(char));
                                        sprintf(msgStr,"Game ended. Opponent left!");
                                        printStrAtConsolePos(consoleScr,0,0,msgStr,BLACK_ON_WHITE);
                                        drawMenu(consoleScr, &menus[currentMenuId], currentSubmenuIndex, 2, 2);
                                        free(msgStr);
                                        break;

                                    case 2:
                                        system("cls");
                                        inGame = 0;
                                        awaitingMove = 0;
                                        freeGameBoard(currentBoard);
                                        msgStr = calloc(64,sizeof(char));
                                        sprintf(msgStr,"Game ended. Player %s won!", lastPacket->packetData+2);
                                        printStrAtConsolePos(consoleScr,0,0,msgStr,BLACK_ON_WHITE);
                                        drawMenu(consoleScr, &menus[currentMenuId], currentSubmenuIndex, 2, 2);
                                        free(msgStr);
                                        int ch;
                                        //if()
                                        break;
                                    case 3:
                                        system("cls");
                                        inGame = 0;
                                        awaitingMove = 0;
                                        freeGameBoard(currentBoard);
                                        msgStr = calloc(64,sizeof(char));
                                        sprintf(msgStr,"Game ended. Its a tie!");
                                        printStrAtConsolePos(consoleScr,0,0,msgStr,BLACK_ON_WHITE);
                                        drawMenu(consoleScr, &menus[currentMenuId], currentSubmenuIndex, 2, 2);
                                        free(msgStr);
                                        break;
                                }
                            }
                            break;

                        default:
                            break;
                    }
                    break;

                case DataSendPacket:
                    switch(lastPacket->packetSubtype)
                    {
                        case SendPlayerMove:
                            awaitingMove = 1;
                            makeMove(currentBoard,lastPacket->packetData[0] - 1, lastPacket->packetData[1]-1,oppCellType);
                            boardRedraw = 1;
                            currentBoardColumn = 1;
                            currentBoardRow = 1;
                            break;
                    }
                    break;

                case DataRequestPacket:
                    switch(lastPacket->packetSubtype)
                    {
                        case RequestPlayerMove:
                            //printf("RequestMove");
//                            awaitingMove = 1;
//                            GAME_BOARD* newBoard = decodeBoard(lastPacket->packetData);
//                            memcpy(currentBoard->board,newBoard->board,9);
//                            freeGameBoard(newBoard);
//                            boardRedraw = 1;
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
            freePacket(lastPacket);
        }

        if(inGame)
        {
            if (boardRedraw == 1) {
                draw3x3BoardAt(consoleScr, currentBoard, currentBoardRow, currentBoardColumn, 2, 2);
                if(awaitingMove == 1)
                {
                    char* notifStr = calloc(64, sizeof(char));
                    if(myCellType == ZeroCell) {
                        sprintf(notifStr, "Make your move... You sign is %c", 'O');
                    }else
                    {
                        sprintf(notifStr, "Make your move... You sign is %c", 'X');
                    }
                    printStrAtConsolePos(consoleScr,0,0,notifStr,BLACK_ON_WHITE);
                    free(notifStr);
                }
                else
                {
                    char* notifStr = calloc(64, sizeof(char));
                    if(myCellType == ZeroCell) {
                        sprintf(notifStr, "Waiting for opponent's move... You sign is %c", 'O');
                    }else
                    {
                        sprintf(notifStr, "Waiting for opponent's move... You sign is %c", 'X');
                    }
                    printStrAtConsolePos(consoleScr,0,0,notifStr,BLACK_ON_WHITE);
                    free(notifStr);
                }
                boardRedraw = 0;
            }
        }
    }

    return 0;
}

void ErrorExit (LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);

    ExitProcess(0);
}

//void KeyEventProc(KEY_EVENT_RECORD ker)
//{
//    printf("Key event: ");
//}
#pragma clang diagnostic pop