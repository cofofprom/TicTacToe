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

    char testData[] = {1,2,3,4,5,6,7,8,9,10,0};

    char dump = ' ';

    PACKET* testPacket = initPacketFromParams(ServicePacket,ServiceUserAction,BlacklistAction,&testData);
    transmitPacketWithClientWorker(worker, testPacket);

    dump = getchar();
    PACKET* recPacket = getPacketFromClientWorker(worker,NULL);
    dump = getchar();

    stopNetworkWorker(worker);
    freePacket(testPacket);

    return 0;
}