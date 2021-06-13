#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <winsock2.h>
#include <string.h>
#include "..\clientNetworking\packet.h"
#include "..\clientNetworking\packetEnums.h"
#include "utils.h"

#define SIP "127.0.0.1"
#define SPORT 80
#define INBUFSIZE 1024

SERVERUSER_LITE* users;

void* processRequest(void* arg)
{
    SOCKET* client = (SOCKET*)arg;
    if (preLoginRoutine(client) == SOCKET_ERROR) return 0;
    int loginWorkflowFlag = 1;
    while(loginWorkflowFlag) {
        char rawData[INBUFSIZE] = {0};
        recv(*client, rawData, INBUFSIZE, 0);
        PACKET *inputData = decodePacket(rawData);
        if (inputData->packetSubtype == SendLoginData) {
            char loginLen = inputData->packetData[0];
            char passLen = inputData->packetData[loginLen + 1];
            char* login = (char*)calloc(loginLen + 1, sizeof(char));
            char* pass = (char*)calloc(passLen + 1, sizeof(char));
            for(int i = 0; i < loginLen; i++) {
                login[i] = inputData->packetData[i + 1];
            }
            for(int i = 0; i < passLen; i++) {
                pass[i] = inputData->packetData[loginLen + i + 2];
            }
            // СДЕЛАТЬ НОРМ ЛОГИН
            if (!strcmp(login, "amogus") && !strcmp(pass, "oralcumshot")) {
                loginWorkflowFlag = 0;
            }
        } else if (inputData->packetSubtype == RequestCheckUsername) {
            // если надо чекнуть нейм
        } else if (inputData->packetSubtype == SendRegisterRequest) {
            // если надо регаться

        } else {
            // хуйня
        }
    }
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
    SOCKET client = accept(serverSock, (SOCKADDR*)&clientData, &clientDataLen);
    pthread_t  thread;
    pthread_create(&thread, NULL, processRequest, (void*)&client);
    pthread_join(thread, NULL);
    return 0;
}