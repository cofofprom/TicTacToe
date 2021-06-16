#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <windows.h>
#include <winsock.h>
#include <time.h>

#include "../../clientNetworking/clientNetworkWorker.h"

int main()
{
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0)
    {
        printf("Can't connect to socket lib");
        return 1;
    }

    int errCode = 0;

    NETWORK_WORKER* worker = initNewNetworkWorker(workerMain,"127.0.0.1",5510,&errCode);

    char login[] = {1,2,3,4,5,0};
    char password[] = {5,4,3,2,1,0};
    char testNickname[] = {1,2,3,4,5,6,7,8,9,10,0};
    char boardSize = 5;
    char row = 3;
    char column = 2;

    sendDisconnect(worker);

    char dump = ' ';

    dump = getchar();
//    PACKET* recPacket = getPacketFromClientWorker(worker,NULL);
//    dump = getchar();

    stopNetworkWorker(worker);
//    freePacket(testPacket);

    return 0;
}