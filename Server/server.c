#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <winsock2.h>

#define SIP "127.0.0.1"
#define SPORT 80

void* processRequest(void* arg)
{
    printf("Hello request");
}

int main(int argc,char** argv)
{
    WSADATA sdata;
    WSAStartup(MAKEWORD(2, 2), &sdata);
    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addr;
    addr.sin_port = htons(SPORT);
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(SIP);
    bind(serverSock, (SOCKADDR*)&addr, sizeof(addr));
    listen(serverSock, 100);
    SOCKADDR_IN clientData;
    int clientDataLen;
    accept(serverSock, (SOCKADDR*)&clientData, &clientDataLen);
    pthread_t  thread;
    pthread_create(&thread, NULL, processRequest, (void*)&clientData);
    pthread_join(thread, NULL);
    return 0;
}