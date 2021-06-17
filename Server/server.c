#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <winsock2.h>
#include <string.h>
#include "..\clientNetworking\packet.h"
#include "..\clientNetworking\packetEnums.h"
#include "utils.h"
//#include "..\gameboard\gameboard.h"

#define SIP "127.0.0.1"
#define SPORT 5510
#define INBUFSIZE 1024
#define USRCOUNT 100

SERVERUSER_LITE USR[USRCOUNT] = { 0 };
int USRSZ = 0;

void* processRequest(void* arg)
{
    printf("%s\n", "someone connected");
    SOCKET* client = (SOCKET*)arg;
    int curr;
    int winner;
    if (preLoginRoutine(client) == SOCKET_ERROR) return 0;
    char rawData[INBUFSIZE] = {0};
    if(recv(*client, rawData, INBUFSIZE, 0) == SOCKET_ERROR) printf("%d\n", WSAGetLastError());
    PACKET *inputData = decodePacket(rawData);
    if (inputData->packetSubtype == SendLoginData) {
        char loginLen = inputData->packetData[0];
        char* login = (char*)calloc(loginLen + 1, sizeof(char));
        for(int i = 0; i < loginLen; i++) {
            login[i] = inputData->packetData[i + 1];
        }
        login[loginLen] = 0;
        printf("login is %s\n", login);
        USR[USRSZ++] = *initUser(login, *client); // юзверь авторизован
        curr = USRSZ - 1;
        //send(USR[curr].usersock, login, strlen(login), 0);
        PACKET* ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
        char* okRaw = encodePacket(ok);
        send(*client, okRaw, strlen(okRaw), 0);
    }
    int exitFlag = 0;
    //char boardSize;
    int opponent;
    while(!exitFlag) {
        char rawData[INBUFSIZE] = {0};
        int rawdataSize = recv(USR[curr].usersock, rawData, INBUFSIZE, 0);
        PACKET *inputData = decodePacket(rawData);
        printf("thread of %s\n", USR[curr].nickname);
        if(inputData->packetSubtype == ServiceUserAction && inputData->packetCode == GameRequestAction) {
            //boardSize = inputData->packetData[0];
            int idx = findNickname(USR, USRSZ, (inputData->packetData + 2));
            SOCKET opponent = USR[idx].usersock;
            char* inviteData = (char*)calloc(strlen(USR[curr].nickname) + 2, sizeof(char));
            inviteData[0] = 3;
            inviteData[1] = (char)strlen(USR[curr].nickname);
            for(int i = 0; i < inviteData[1]; i++) {
                inviteData[i + 2] = USR[curr].nickname[i];
            }
            PACKET* otherInvite = initPacketFromParams(ServicePacket, ServiceUserAction, GameRequestAction, inviteData);
            char* rawInvite = encodePacket(otherInvite);
            printf("invite %s -> %s\n", USR[curr].nickname, USR[idx].nickname);
            send(opponent, rawInvite, strlen(rawInvite), 0);
            USR[curr].role = CrossCell;
            USR[curr].opponentID = idx;
            USR[curr].game = initNewBoard(3);
        }
        else if(inputData->packetSubtype == ServiceUserAction && inputData->packetCode == GameAcceptAction) {
            USR[curr].game = initNewBoard(3);
            USR[curr].role = ZeroCell;
            for(int i = 0; i < USRSZ; i++) {
                if(USR[i].opponentID == curr) {
                    USR[curr].opponentID = i;
                    break;
                }
            }
            printf("accept %s -> %s\n", USR[curr].nickname, USR[USR[curr].opponentID].nickname);
            PACKET* ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
            char* okRaw = encodePacket(ok);
            send(USR[USR[curr].opponentID].usersock, okRaw, strlen(okRaw), 0);
            printf("%s playing with %s\n", USR[curr].nickname, USR[USR[curr].opponentID].nickname);
            printf("curr is %d, opponent is %d", USR[curr].role, USR[USR[curr].opponentID].role);
            char* boardEncoding = encodeBoard(USR[curr].game);
            PACKET* requestMove = initPacketFromParams(DataRequestPacket, RequestPlayerMove, 0, boardEncoding);
            char* requestMoveRaw = encodePacket(requestMove);
            send(USR[curr].usersock, requestMoveRaw, strlen(requestMoveRaw), 0);
            printf("\nrequest move -> %s\n", USR[curr].nickname);
        }
        else if(inputData->packetSubtype == SendPlayerMove) {
            char x = inputData->packetData[0];
            char y = inputData->packetData[1];
            printf("%s moving on %d, %d. Sending this to %s\n", USR[curr].nickname, x, y, USR[USR[curr].opponentID].nickname);
            send(USR[USR[curr].opponentID].usersock, rawData, rawdataSize, 0);
            PACKET* ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
            char* okRaw = encodePacket(ok);
            send(USR[curr].usersock, okRaw, strlen(okRaw), 0);
            makeMove(USR[curr].game, x, y, USR[curr].role);
            makeMove(USR[USR[curr].opponentID].game, x, y, USR[curr].role);
            char* boardEncoding = encodeBoard(USR[curr].game);
            PACKET* requestMove = initPacketFromParams(DataRequestPacket, RequestPlayerMove, 0, boardEncoding);
            char* requestMoveRaw = encodePacket(requestMove);
            send(USR[USR[curr].opponentID].usersock, requestMoveRaw, strlen(requestMoveRaw), 0);
        }
        winner = checkBoardWinConditions(USR[curr].game);
        if(USR[curr].game && winner != GAMEBOARD_NO_WIN) exitFlag = 1;
    }
    if(USR[curr].role == winner) printf("%s has won!!\n", USR[curr].nickname);
    printf("someone has won!\n");
}

int main(int argc,char** argv)
{
    /*char logindata[2] = {2, 2};
    PACKET* sendLoginData = initPacketFromParams(DataSendPacket, SendPlayerMove, 0, logindata);
    char* enc = encodePacket(sendLoginData);*/
    WSADATA sdata;
    WSAStartup(MAKEWORD(2, 2), &sdata);
    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addr;
    addr.sin_port = htons(SPORT);
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(SIP);
    bind(serverSock, (SOCKADDR*)&addr, sizeof(addr));
    listen(serverSock, 100);
    while(1) {
        SOCKADDR_IN clientData;
        int clientDataLen;
        SOCKET client = accept(serverSock, (SOCKADDR *) &clientData, &clientDataLen);
        pthread_t thread;
        pthread_create(&thread, NULL, processRequest, (void *) &client);
        //pthread_join(thread, NULL);
    }
    return 0;
}