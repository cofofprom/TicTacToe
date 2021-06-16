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
#define SPORT 80
#define INBUFSIZE 1024
#define USRCOUNT 100

SERVERUSER_LITE USR[USRCOUNT] = { 0 };
int USRSZ = 0;

void* processRequest(void* arg)
{
    printf("%s\n", "someone connected");
    SOCKET* client = (SOCKET*)arg;
    int curr;
    if (preLoginRoutine(client) == SOCKET_ERROR) return 0;
    char rawData[INBUFSIZE] = {0};
    if(recv(*client, rawData, INBUFSIZE, 0) == SOCKET_ERROR) printf("%d\n", WSAGetLastError());
    printf("amogus = '%s'\n", rawData);
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
        PACKET* ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
        char* okRaw = encodePacket(ok);
        send(*client, okRaw, strlen(okRaw), 0);
    }

    /*int exitFlag = 0;
    //char boardSize;
    int opponent;
    while(!exitFlag) {
        char rawData[INBUFSIZE] = {0};
        recv(*client, rawData, INBUFSIZE, 0);
        PACKET *inputData = decodePacket(rawData);
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
            PACKET* ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
            char* okRaw = encodePacket(ok);
            send(USR[USR[curr].opponentID].usersock, okRaw, strlen(okRaw), 0);
            PACKET* requestMove = initPacketFromParams(DataRequestPacket, RequestPlayerMove, 0, encodeBoard(USR[curr].game));
            char* requestMoveRaw = encodePacket(requestMove);
            send(USR[USR[curr].opponentID].usersock, requestMoveRaw, strlen(requestMoveRaw), 0);
        }
        else if(USR[curr].game != NULL && inputData->packetSubtype == SendPlayerMove) {
            char x = inputData->packetData[0];
            char y = inputData->packetData[1];
            send(USR[USR[curr].opponentID].usersock, rawData, strlen(rawData), 0);
            PACKET* ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
            char* okRaw = encodePacket(ok);
            send(*client, okRaw, strlen(okRaw), 0);
            makeMove(USR[curr].game, x, y, USR[curr].role);
            makeMove(USR[USR[curr].opponentID].game, x, y, USR[curr].role);
            PACKET* requestMove = initPacketFromParams(DataRequestPacket, RequestPlayerMove, 0, encodeBoard(USR[curr].game));
            char* requestMoveRaw = encodePacket(requestMove);
            send(USR[USR[curr].opponentID].usersock, requestMoveRaw, strlen(requestMoveRaw), 0);
        }

        if(USR[curr].game && checkBoardWinConditions(USR[curr].game) != GAMEBOARD_NO_WIN) exitFlag = 1;
    }

    /* else if (inputData->packetSubtype == RequestCheckUsername) {
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
    }*/
    /*
    int gamepreparationflag = 1;
    char rawDataBuf[INBUFSIZE] = { 0 };
    GAME_BOARD* target;
    while(gamepreparationflag) {
        recv(*client, rawDataBuf, INBUFSIZE, 0);
        PACKET* leninaPaket = decodePacket(rawDataBuf);
        if (leninaPaket->packetCode == GameRequestAction) {
            char boardSize = leninaPaket->packetData[0];
            char nickLen = leninaPaket->packetData[1];
            char* nick = (char*)calloc(nickLen + 1, sizeof(char));
            char* outdata = (char*)calloc(nickLen + 2, sizeof(char));
            outdata[0] = boardSize;
            outdata[1] = nickLen;
            for(int i = 0; i < nickLen; i++) {
                nick[i] = leninaPaket->packetData[i + 2];
                outdata[i+2] = leninaPaket->packetData[i + 2];
            }
            SERVERUSER_LITE* opponent = &users[findNickname(users, usersz, nick)];
            PACKET* serverInvite = initPacketFromParams(ServicePacket, ServiceUserAction, GameRequestAction, outdata);
            char* rawinvite = encodePacket(serverInvite);
            send(opponent->usersock, rawinvite, strlen(rawinvite), 0);
            char rawRespBuf[INBUFSIZE] = { 0 };
            recv(opponent->usersock, rawRespBuf, INBUFSIZE, 0);
            PACKET* confirmation = decodePacket(rawRespBuf);
            if(confirmation->packetCode == GameAcceptAction) {
                PACKET* ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
                char* rawok = encodePacket(ok);
                send(*client, rawok, strlen(rawok), 0);
                gamepreparationflag = 0;
                target = initNewBoard(boardSize);
            }
        }
    }

    int gameflag = 0;*/
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