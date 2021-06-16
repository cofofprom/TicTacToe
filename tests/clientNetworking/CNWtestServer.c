#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <windows.h>
#include <winsock.h>

#include "../../clientNetworking/clientNetworkWorker.h"

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

void* ClientStart(void* param) {
    SOCKET client = (SOCKET) param;
    char recieve[1024], transmit[1024], temp[2];
    int ret;
    while(1)
    {
        ret = recv(client, recieve, 1024, 0);
        if (!ret || ret == SOCKET_ERROR) {
            printf("Error getting data\n");
            return (void *) 1;
        }
        recieve[ret] = '\0';
        //pthread_mutex_lock(&mutex);
        //pthread_mutex_lock(&mutex_file);
        printf("Received: ");
        for (int i = 0; i < strlen(recieve); i++) {
            printf("%d,",recieve[i]);
        }
        printf("\n");
        //pthread_mutex_unlock(&mutex_file);
        //pthread_mutex_unlock(&mutex);
        //Sleep(2000);

//        char testData[] = {1,2,3,4,5,0};
//
//        PACKET* newPacket = initPacketFromParams(ServicePacket,ServiceUserAction,BlacklistAction,&testData);
//
//        char* encoding = encodePacket(newPacket);

//        printf("Transmitting: ");
//        for (int i = 0; i < strlen(encoding); i++) {
//            printf("%d,",encoding[i]);
//        }
//        printf("\n");

//        ret = send(client, encoding, strlen(encoding), 0);
//        if (ret == SOCKET_ERROR)
//        {
//            pthread_mutex_lock(&mutex);
//            pthread_mutex_lock(&mutex_file);
//            printf("Error sending data\n");
//            pthread_mutex_unlock(&mutex_file);
//            pthread_mutex_unlock(&mutex);
//            return (void*)2;
//        }

//        freePacket(newPacket);
//        free(encoding);
    }
    return (void*)0;
}

int CreateServer()
{
    SOCKET server, client;
    struct sockaddr_in localaddr, clientaddr;
    int size;
    server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (server == INVALID_SOCKET)
    {
        printf("Error create server\n");
        return 1;
    }
    localaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(5510);//port number is for example, must be more than 1024
    if (bind(server, (struct sockaddr*)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
    {
        printf("Can't start server\n");
        return 2;
    }
    else
    {
        printf("Server is started\n");
    }
    listen(server, 50);//50 клиентов в очереди могут стоять
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_file, NULL);
    int result = 1;
    while (result)
    {
        size = sizeof(clientaddr);
        client = accept(server, (struct sockaddr*)&clientaddr, &size);

        if (client == INVALID_SOCKET)
        {
            printf("Error accept client\n");
            continue;
        }
        else
        {
            printf("Client is accepted\n");
        }
        pthread_t mythread;
        int status = pthread_create(&mythread, NULL, ClientStart, (void*)client);
        pthread_detach(mythread);
    }
    pthread_mutex_destroy(&mutex_file);
    pthread_mutex_destroy(&mutex);
    printf("Server is stopped\n");
    closesocket(server);
    return 0;
}

int main()
{
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0)
    {
        printf("Connected to socket lib\n");
    }
    else
    {
        return 1;
    }
    return CreateServer();
}