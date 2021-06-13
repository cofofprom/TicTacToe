#ifndef TICTACTOE_CLIENTNETWORKWORKER_H
#define TICTACTOE_CLIENTNETWORKWORKER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <windows.h>
#include <winsock.h>

typedef struct client_worker_data
{
    pthread_t workerThread;
    pthread_mutex_t workerDataMutex;
    SOCKET workerSocket;
    int workerPort;
}NETWORK_WORKER;

NETWORK_WORKER* initNewNetworkWorker();

#endif //TICTACTOE_CLIENTNETWORKWORKER_H
