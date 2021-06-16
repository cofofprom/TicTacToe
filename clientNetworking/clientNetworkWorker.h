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

#include "packetQueue.h"


#define CNW_SUCCESS 0
#define CNW_NULL_PTR_ERR -1
#define CNW_PORT_OUT_OF_RANGE -2
#define CNW_ERROR_SOCKET_CREATION -3
#define CNW_SERVER_CONNECTION_ERROR -4
#define CNW_MUTEX_CREATION_ERROR -5
#define CNW_WINSOCK_INIT_FAILED -6
#define CNW_SOCKET_NOBLOCKING_MODE_FAILED -7
#define CNW_QUEUE_EMPTY -8
#define CNW_SEND_ERROR -9
#define CNW_QUEUE_FULL -10

///@par NetworkWorker:
///Use network worker to receive and transmit network packets.
///All received packets are stored in worker's receive queue.
///All packets that need to be transmitted should be enqueued to worker's transmission queue

///@brief Struct for storing network worker parameters
///@param workerThread Thread that this worker is running on
///@param workerTransmitQueueMutex Mutex to control this worker's transmit queue
///@param workerReceiveQueueMutex Mutex to control this worker's receive queue
///@param workerSocketMutex Mutex to control this worker's socket
///@param workerSocket Socket this worker is using
///@param receivedPacketQueue Queue where all received packets are stored
///@param transmittingPacketsQueue Queue where packets awaiting transmission are stored
typedef struct client_worker_data
{
    pthread_t workerThread;
    pthread_mutex_t transmitPacketQueueMutex;
    pthread_mutex_t receivedPacketQueueMutex;
    pthread_mutex_t socketMutex;
    SOCKET networkSocket;
    struct sockaddr_in serverAddress;
    PACKET_QUEUE* receivedPacketQueue;
    PACKET_QUEUE* transmitPacketQueue;
    int stopFlag;
    int lastError;
    int errorCount;
}NETWORK_WORKER;

///@brief Initializes new network worker with main function clientWorkerMain on specified port
///@param clientWorkerMain Main function for this worker's workerThread
///@param address Address for this worker's socket. Should be formatted as "xxx.xxx.xxx.xxx" string
///@param port Port for this worker's socket
///@param errorCode Pointer to int, where error code will be stored in case of error. If NULL, it is ignored
///@retval NULL On initialization error
///@retval NETWORK_WORKER* On success
NETWORK_WORKER* initNewNetworkWorker(void* (*clientWorkerMain)(void*),const char* address, unsigned short port, int* errorCode);

///@brief Stops target network worker and frees its memory
void stopNetworkWorker(NETWORK_WORKER* targetWorker);

///@brief Stock network worker main function. Use this, if no special functionality is required
///@param workerData Data for worker that uses this function
///@retval status code upon exiting
void* workerMain(void* attr);

///@brief Function to request packet transmission from worker.
///@param targetWorker Worker that is used for transmission
///@param targetPacket Packet to be transmitted
///@retval CNW_SUCCESS On success
///@retval CNW_QUEUE_FULL If transmission queue is full
///@retval CNW_NULL_PTR_ERR If null pointer is encountered
int transmitPacketWithClientWorker(NETWORK_WORKER* targetWorker, PACKET* targetPacket);

///@brief Get one received packet from worker's receiving queue
///@param targetWorker Worker to get packet from
///@param errorCode Pointer to int, where error code would be reported. If NULL, it is ignored
///@retval NULL On error or if receiving queue is empty. Queue length check should be performed to detect errors through this value.
///@retval PACKET* Packet from the head of worker's receiving queue
PACKET* getPacketFromClientWorker(NETWORK_WORKER* targetWorker, int* errorCode);

#endif //TICTACTOE_CLIENTNETWORKWORKER_H
