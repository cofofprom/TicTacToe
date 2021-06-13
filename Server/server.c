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
#define USRCOUNT 100

SERVERUSER_LITE users[USRCOUNT] = { 0 };
int usersz = 0;

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
            for(int i = 0; i < usersz; i++) {
                if(!strcmp(login, users[i].nickname) && !strcmp(pass, users[i].password)) {
                    loginWorkflowFlag = 0;
                    users[i].isAuth = 1;
                    users[i].usersock = *client;
                }
            }
        } else if (inputData->packetSubtype == RequestCheckUsername) {
            int okflag = checkNickname(users, usersz, inputData->packetData);
            if(okflag) {
                PACKET* success = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
                char* raw = encodePacket(success);
                send(*client, raw, (int)strlen(raw), 0);
            } else {
                PACKET* err = initPacketFromParams(ServicePacket, ServiceErrorPacket, NoRegUsernameTakenErr, 0);
                char* raw = encodePacket(err);
                send(*client, raw, (int)strlen(raw), 0);
            }

        } else if (inputData->packetSubtype == SendRegisterRequest) {
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
            if(checkNickname(users, usersz, login)) {
                users[usersz++] = *deserialize(inputData->packetData);
                PACKET* success = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
                char* raw = encodePacket(success);
                send(*client, raw, (int)strlen(raw), 0);
                loginWorkflowFlag = 0;
                users[usersz - 1].isAuth = 1;
                users[usersz - 1].usersock = *client;
            }
            else {
                PACKET* err = initPacketFromParams(ServicePacket, ServiceErrorPacket, NoRegUsernameTakenErr, 0);
                char* raw = encodePacket(err);
                send(*client, raw, (int)strlen(raw), 0);
            }
        } else {
            // хуйня
        }
    }
}

int main(int argc,char** argv)
{
    FILE* db = fopen("usr.db", "rb");
    char cusr[1024] = {0};
    while(fgets(cusr, 1024, db) != NULL) {
        users[usersz++] = *deserialize(cusr);
    }
    fclose(db);
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
    FILE* dbw = fopen("usr.db", "wb");
    for(int i = 0; i < usersz; i++) {
        char* buf = serialize(&users[usersz]);
        fprintf(dbw, "%s\n", buf);
    }
    fclose(dbw);
    return 0;
}