#include "clientNetworkWorker.h"

NETWORK_WORKER *initNewNetworkWorker(void* (*clientWorkerMain)(void*), const char *address, unsigned short port, int* errorCode) {
    if(clientWorkerMain == NULL || address == NULL)
    {
        if(errorCode != NULL) *errorCode = CNW_NULL_PTR_ERR;
        return NULL;
    }
    if(port > 65535)
    {
        if(errorCode != NULL) *errorCode = CNW_PORT_OUT_OF_RANGE;
        return NULL;
    }

    NETWORK_WORKER* newWorker = calloc(1, sizeof(NETWORK_WORKER));

    int result = 0;

    //Socket initialization
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0)
    {
        if(errorCode) *errorCode = CNW_WINSOCK_INIT_FAILED;
        free(newWorker);
        return NULL;
    }

    newWorker->networkSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (newWorker->networkSocket == INVALID_SOCKET)
    {
        if(errorCode != NULL) *errorCode = CNW_ERROR_SOCKET_CREATION;
        free(newWorker);
        return NULL;
    }
    u_long socketMode = 1;
    result = ioctlsocket(newWorker->networkSocket, FIONBIO, &socketMode);
    if (result != NO_ERROR)
    {
        if(errorCode) *errorCode = CNW_SOCKET_NOBLOCKING_MODE_FAILED;
        free(newWorker);
        return NULL;
    }

    newWorker->serverAddress.sin_family = AF_INET;
    newWorker->serverAddress.sin_port = htons(port);
    newWorker->serverAddress.sin_addr.S_un.S_addr = inet_addr(address);
    result = connect(newWorker->networkSocket, (struct sockaddr*)&newWorker->serverAddress, sizeof(newWorker->serverAddress));
    if (result == SOCKET_ERROR)
    {
        result = WSAGetLastError();
        if(result != WSAEWOULDBLOCK)
        {
            if(errorCode) errorCode = CNW_SERVER_CONNECTION_ERROR;
            closesocket(newWorker->networkSocket);
            free(newWorker);
            return NULL;
        }

    }

    //Mutex creation
    result = pthread_mutex_init(&newWorker->receivedPacketQueueMutex, NULL);
    if(result)
    {
        if(errorCode) *errorCode = CNW_MUTEX_CREATION_ERROR;
        closesocket(newWorker->networkSocket);
        free(newWorker);
        return 0;
    }

    result = pthread_mutex_init(&newWorker->transmitPacketQueueMutex, NULL);
    if(result)
    {
        if(errorCode) *errorCode = CNW_MUTEX_CREATION_ERROR;
        closesocket(newWorker->networkSocket);
        free(newWorker);
        return 0;
    }

    result = pthread_mutex_init(&newWorker->socketMutex, NULL);
    if(result)
    {
        if(errorCode) *errorCode = CNW_MUTEX_CREATION_ERROR;
        closesocket(newWorker->networkSocket);
        free(newWorker);
        return 0;
    }

    newWorker->transmitPacketQueue = initPacketQueue();
    newWorker->receivedPacketQueue = initPacketQueue();

    pthread_create(&newWorker->workerThread,NULL,clientWorkerMain,newWorker);

    return newWorker;
}

void stopNetworkWorker(NETWORK_WORKER *targetWorker) {
    if(targetWorker == NULL)
    {
        return;
    }

    targetWorker->stopFlag = 1;
    //Wait until thread finishes
    pthread_join(targetWorker->workerThread,NULL);
    closesocket(targetWorker->networkSocket);
    freePacketQueue(targetWorker->transmitPacketQueue);
    freePacketQueue(targetWorker->receivedPacketQueue);
    pthread_mutex_destroy(&targetWorker->receivedPacketQueueMutex);
    pthread_mutex_destroy(&targetWorker->transmitPacketQueueMutex);
    free(targetWorker);

    return;
}

void* workerMain(void* attr) {
    //Init
    NETWORK_WORKER* targetWorker = (NETWORK_WORKER*) attr;
    int result = 0;

    short incomingBytesLeft = 0;
    char* currentIncomingEncoding = NULL;

    char* packetStart = calloc(3, sizeof(char));

    //Loop
    while(targetWorker->stopFlag != 1)
    {
        //Check if we have packets in transmission queue. Mutex are not needed during check due to read-only access
        if(targetWorker->transmitPacketQueue->length > 0)
        {
            pthread_mutex_lock(&targetWorker->transmitPacketQueueMutex);
            PACKET* currentPacket = dequeuePacket(targetWorker->transmitPacketQueue);
            pthread_mutex_unlock(&targetWorker->transmitPacketQueueMutex);

            char* encoding = encodePacket(currentPacket);

            pthread_mutex_lock(&targetWorker->socketMutex);
            result = send(targetWorker->networkSocket,encoding,strlen(encoding),0);
            pthread_mutex_unlock(&targetWorker->socketMutex);
            if(result == SOCKET_ERROR)
            {
                targetWorker->lastError = CNW_SEND_ERROR;
                targetWorker->errorCount++;
            }
        }

        pthread_mutex_lock(&targetWorker->socketMutex);
        result = recv(targetWorker->networkSocket,packetStart,2,MSG_PEEK);
        pthread_mutex_unlock(&targetWorker->socketMutex);


        //If there are no bytes available, or we are waiting for a new packet but its length is not fully available, continue onto next cycle
        if(result == SOCKET_ERROR || (result < 2 && currentIncomingEncoding == NULL))
        {
            continue;
        }

        //There are at least 2 byte available, or we are already decoding a packet.
        //If we are waiting for a new packet, these bytes are encoding its length. Decode them and continue onto the next cycle
        if(currentIncomingEncoding == NULL)
        {
            pthread_mutex_lock(&targetWorker->socketMutex);
            result = recv(targetWorker->networkSocket,packetStart,2,0);
            pthread_mutex_unlock(&targetWorker->socketMutex);
            packetStart[0]--;
            packetStart[1]--;
            memcpy(&incomingBytesLeft,packetStart,sizeof(short));
            currentIncomingEncoding = calloc(incomingBytesLeft + 2 + 1,sizeof(char));
            memcpy(currentIncomingEncoding,packetStart,sizeof(short));
            currentIncomingEncoding[0]++;
            currentIncomingEncoding[1]++;
            continue;
        }

        //If we are already reading a packet, wait until it is fully available, and read it
        pthread_mutex_lock(&targetWorker->socketMutex);
        result = recv(targetWorker->networkSocket,currentIncomingEncoding+sizeof(short),incomingBytesLeft,MSG_PEEK);
        pthread_mutex_unlock(&targetWorker->socketMutex);
        if(result >= incomingBytesLeft && currentIncomingEncoding != NULL)
        {
            pthread_mutex_lock(&targetWorker->socketMutex);
            result = recv(targetWorker->networkSocket,currentIncomingEncoding+sizeof(short),incomingBytesLeft,0);
            pthread_mutex_unlock(&targetWorker->socketMutex);
            PACKET* newPacket = decodePacket(currentIncomingEncoding);
            pthread_mutex_lock(&targetWorker->receivedPacketQueueMutex);
            enqueuePacket(targetWorker->receivedPacketQueue,newPacket);
            pthread_mutex_unlock(&targetWorker->receivedPacketQueueMutex);
            free(currentIncomingEncoding);
            currentIncomingEncoding = NULL;
            incomingBytesLeft = 0;
            //printf("Worker: packet decoded");
            continue;
        }
    }

    //Cleaning up and finishing thread
    return CNW_SUCCESS;
}

int transmitPacketWithClientWorker(NETWORK_WORKER *targetWorker, PACKET *targetPacket) {
    if(targetWorker == NULL || targetPacket == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    int result;

    pthread_mutex_lock(&targetWorker->transmitPacketQueueMutex);
    result = enqueuePacket(targetWorker->transmitPacketQueue,targetPacket);
    pthread_mutex_unlock(&targetWorker->transmitPacketQueueMutex);
    if(result != PACKETQUEUE_SUCCESS)
    {
        switch(result)
        {
            case PACKETQUEUE_QUEUE_FULL:
                return CNW_QUEUE_FULL;
                break;

            case PACKETQUEUE_NULL_PTR:
                return CNW_NULL_PTR_ERR;
                break;
        }
    }

    return CNW_SUCCESS;
}

PACKET *getPacketFromClientWorker(NETWORK_WORKER *targetWorker, int* errorCode) {
    if(targetWorker == NULL)
    {
        if(errorCode) *errorCode = CNW_NULL_PTR_ERR;
        return NULL;
    }
    pthread_mutex_lock(&targetWorker->receivedPacketQueueMutex);
    if(targetWorker->receivedPacketQueue->length == 0)
    {
        if(errorCode) *errorCode = CNW_QUEUE_EMPTY;
        return NULL;
    }
    PACKET* newPacket = dequeuePacket(targetWorker->receivedPacketQueue);
    pthread_mutex_unlock(&targetWorker->receivedPacketQueueMutex);

    return newPacket;
}

int sendRegisterData(NETWORK_WORKER *targetWorker, char *login, char *password) {
    if(targetWorker == NULL || login == NULL || password == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    if(strlen(login) > 32 || strlen(password) > 32)
    {
        return CNW_BAD_PACKET_DATA;
    }

    char* packetData = calloc(strlen(login) + strlen(password) + 2 + 1, sizeof(char));
    char* currentDataPos = packetData;

    *currentDataPos = strlen(login);
    currentDataPos++;

    memcpy(currentDataPos,login,strlen(login));
    currentDataPos += strlen(login);

    *currentDataPos = strlen(password);
    currentDataPos++;

    memcpy(currentDataPos,password,strlen(password));
    currentDataPos += strlen(password);

    PACKET* newPacket = initPacketFromParams(DataSendPacket, SendRegisterRequest, 0, packetData);
    transmitPacketWithClientWorker(targetWorker,newPacket);

    freePacket(newPacket);
    free(packetData);

    return CNW_SUCCESS;
}

int sendLoginData(NETWORK_WORKER *targetWorker, char *login, char *password) {
    if(targetWorker == NULL || login == NULL || password == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    if(strlen(login) > 32 || strlen(password) > 32)
    {
        return CNW_BAD_PACKET_DATA;
    }

    char* packetData = calloc(strlen(login) + strlen(password) + 2 + 1, sizeof(char));
    char* currentDataPos = packetData;

    *currentDataPos = strlen(login);
    currentDataPos++;

    memcpy(currentDataPos,login,strlen(login));
    currentDataPos += strlen(login);

    *currentDataPos = strlen(password);
    currentDataPos++;

    memcpy(currentDataPos,password,strlen(password));
    currentDataPos += strlen(password);

    PACKET* newPacket = initPacketFromParams(DataSendPacket, SendLoginData, 0, packetData);
    transmitPacketWithClientWorker(targetWorker,newPacket);

    freePacket(newPacket);
    free(packetData);

    return CNW_SUCCESS;
}

int sendBoardMove(NETWORK_WORKER *targetWorker, char row, char column) {
    if(targetWorker == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    char* packetData = calloc(3,sizeof(char));

    packetData[0] = row + 1;
    packetData[1] = column + 1;

    PACKET* newPacket = initPacketFromParams(DataSendPacket,SendPlayerMove,0,packetData);
    transmitPacketWithClientWorker(targetWorker,newPacket);

    freePacket(newPacket);
    free(packetData);

    return CNW_SUCCESS;
}

int requestFriendList(NETWORK_WORKER *targetWorker) {
    if(targetWorker == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    PACKET* newPacket = initPacketFromParams(DataRequestPacket,RequestPlayerList,2,NULL);

    transmitPacketWithClientWorker(targetWorker, newPacket);

    freePacket(newPacket);

    return CNW_SUCCESS;
}

int requestGame(NETWORK_WORKER *targetWorker,char boardSize, char *opponentNickname) {
    if(targetWorker == NULL || opponentNickname == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    char* packetData = calloc(strlen(opponentNickname) + 3, sizeof(char));
    packetData[0] = boardSize;
    packetData[1] = strlen(opponentNickname);
    memcpy(packetData+2, opponentNickname, strlen(opponentNickname));

    PACKET* newPacket = initPacketFromParams(ServicePacket,ServiceUserAction,GameRequestAction,packetData);
    transmitPacketWithClientWorker(targetWorker, newPacket);

    freePacket(newPacket);
    free(packetData);

    return CNW_SUCCESS;
}

int endGame(NETWORK_WORKER *targetWorker) {
    if(targetWorker == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    PACKET* newPacket  = initPacketFromParams(ServicePacket,ServiceEndGame,0,NULL);
    transmitPacketWithClientWorker(targetWorker,newPacket);

    freePacket(newPacket);

    return CNW_SUCCESS;
}

int acceptGameRequest(NETWORK_WORKER *targetWorker, char* opponentNickname) {
    if(targetWorker == NULL || opponentNickname == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    char* packetData = calloc(strlen(opponentNickname) + 2,sizeof(char));
    packetData[0] = strlen(opponentNickname);
    memcpy(packetData+1,opponentNickname,strlen(opponentNickname));

    PACKET* newPacket = initPacketFromParams(ServicePacket,ServiceUserAction,GameAcceptAction,packetData);
    transmitPacketWithClientWorker(targetWorker,newPacket);

    freePacket(newPacket);
    free(packetData);

    return CNW_SUCCESS;
}

int declineGameRequest(NETWORK_WORKER *targetWorker, char* opponentNickname) {
    if(targetWorker == NULL || opponentNickname == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    char* packetData = calloc(strlen(opponentNickname) + 2,sizeof(char));
    packetData[0] = strlen(opponentNickname);
    memcpy(packetData+1,opponentNickname,strlen(opponentNickname));

    PACKET* newPacket = initPacketFromParams(ServicePacket,ServiceUserAction,GameDeclineAction,packetData);
    transmitPacketWithClientWorker(targetWorker,newPacket);

    freePacket(newPacket);
    free(packetData);

    return CNW_SUCCESS;
}

int sendDisconnect(NETWORK_WORKER *targetWorker) {
    if(targetWorker == NULL)
    {
        return CNW_NULL_PTR_ERR;
    }

    PACKET* newPacket  = initPacketFromParams(ServicePacket,ServiceUserAction,DisconnectAction,NULL);
    transmitPacketWithClientWorker(targetWorker,newPacket);

    freePacket(newPacket);

    return CNW_SUCCESS;
}
