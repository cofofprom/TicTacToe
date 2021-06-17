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


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
HANDLE hStdin;
DWORD fdwSaveOldMode;

VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);

int main(int argc, char** argv)
{
    DWORD cNumRead, fdwMode, i;
    INPUT_RECORD irInBuf[128];
    int previousMenuId = -1;
    int currentMenuId = -1;
    int currentSubmenuIndex = -1;
    int DBGDisableLogin = 1;
    int workerErr = 0;
    NETWORK_WORKER* clientWorker = initNewNetworkWorker(workerMain,"127.0.0.1",5510,&workerErr);
    char Title[] = "Tic Tac Toe";
    char* password = calloc(33,sizeof(char));
    char* login = calloc(33,sizeof(char));
    char test[] = "Test\0";

    HANDLE consoleScr = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitle(Title);
    COORD bufferSize = {120, 30};
    SetConsoleScreenBufferSize(consoleScr, bufferSize);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowSize = min(screenWidth,screenHeight) / 1.5;
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    MoveWindow(consoleWindow, WINDOW_SHIFT_X,WINDOW_SHIFT_Y,windowSize,windowSize,TRUE);
    PACKET* lastPacket = initPacket();

    do
    {
        freePacket(lastPacket);
        printf("Enter login:");
        fgets(login,32,stdin);

        printf("Enter password:");
        fgets(password,32,stdin);

        login[strlen(login)-1] = 0;
        password[strlen(password)-1] = 0;

        sendLoginData(clientWorker,login,password);

        while(clientWorker->receivedPacketQueue->length == 0)
        {

        }
        lastPacket = getPacketFromClientWorker(clientWorker,&workerErr);
    } while (lastPacket->packetType != ServicePacket &&
    lastPacket->packetSubtype != ServiceSuccess || ~DBGDisableLogin);

    printf("Successfully logged in! Loading...\n");

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
        if (! ReadConsoleInput(hStdin,irInBuf,128,&cNumRead) ) ErrorExit("ReadConsoleInput");

        // Dispatch the events to the appropriate handler.
        for (i = 0; i < cNumRead; i++)
        {
            switch(irInBuf[i].EventType)
            {
                case KEY_EVENT: // keyboard input
                    KeyEventProc(irInBuf[i].Event.KeyEvent);
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

    return 0;
}

void ErrorExit (LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);

    ExitProcess(0);
}

void KeyEventProc(KEY_EVENT_RECORD ker)
{
    printf("Key event: ");

    if(ker.bKeyDown)
        printf("key pressed \n");
    else printf("key released\n");
}
#pragma clang diagnostic pop