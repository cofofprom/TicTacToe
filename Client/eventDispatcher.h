#ifndef TICTACTOE_EVENTDISPATCHER_H
#define TICTACTOE_EVENTDISPATCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <windows.h>
#include <winsock.h>

typedef struct event_dispatcher_struct
{
    pthread_t workerThread;
    pthread_mutex_t transmitPacketQueueMutex;
    pthread_mutex_t receivedPacketQueueMutex;
    pthread_mutex_t socketMutex;
    SOCKET networkSocket;
    struct sockaddr_in serverAddress;
//    PACKET_QUEUE* receivedPacketQueue;
//    PACKET_QUEUE* transmitPacketQueue;
    int stopFlag;
    int lastError;
    int errorCount;
}EVENT_DISPATCHER;

#endif //TICTACTOE_EVENTDISPATCHER_H
