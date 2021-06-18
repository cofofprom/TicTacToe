#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <winsock2.h>
#include <string.h>
#include "..\clientNetworking\packet.h"
#include "..\clientNetworking\packetEnums.h"
#include "utils.h"
#include "windows.h"
//#include "..\gameboard\gameboard.h"

#define SIP "127.0.0.1"
#define SPORT 5510
#define INBUFSIZE 1024
#define USRCOUNT 100

pthread_mutex_t mutex;
int USRLOCK = 0;
SERVERUSER_LITE USR[USRCOUNT] = { 0 };
int USRSZ = 0;

void* processRequest(void* arg)
{
    SOCKET client = *(SOCKET*)arg;
    int curr;
    int winner1;
    int winner2;
    if (preLoginRoutine(&client) == SOCKET_ERROR) return 0;
    printf("someone connected %d waiting for login...\n", client);
    char rawData[INBUFSIZE] = {0};
    PACKET *inputData;
    printf("%d socket waiting to receive\n", client);
    recvPacket(client, &inputData);
    printf("%d socket received\n", client);

    if (inputData->packetSubtype == SendLoginData) {
        char loginLen = inputData->packetData[0];
        char* login = (char*)calloc(loginLen + 1, sizeof(char));
        for(int i = 0; i < loginLen; i++) {
            login[i] = inputData->packetData[i + 1];
        }
        login[loginLen] = 0;
        pthread_mutex_lock(&mutex);
        USR[USRSZ++] = *initUser(login, client); // юзверь авторизован
        curr = USRSZ - 1;
        printf("login is %s idx is %d sock is %d while inner is %d\n", login, curr, client, USR[curr].usersock);
        pthread_mutex_unlock(&mutex);
        //send(USR[curr].usersock, login, strlen(login), 0);
        PACKET* ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
        char* okRaw = encodePacket(ok);
        pthread_mutex_lock(&mutex);
        if (send(USR[curr].usersock, okRaw, strlen(okRaw), 0) == SOCKET_ERROR)
        {
            pthread_mutex_unlock(&mutex);
            printf("Error in sending smth to usersock\n%s\n", okRaw);
            return 0;
        }
        pthread_mutex_unlock(&mutex);
        printf("USR[%d] is sent\n", curr);
    }
    int exitFlag = 0;
    int endgameconfirmationflag = 0;
    //char boardSize;
    int opponent;

    while(!exitFlag)
    {
        PACKET *inputData;
        //printf("recv of %d\n", client);
        if (recvPacket(client, &inputData) == SOCKET_ERROR)
        {
            pthread_mutex_lock(&mutex);
            USR[curr].online = 0;
            printf("USR[%d] known as '%s' became offline\n", curr, USR[curr].nickname);
            pthread_mutex_unlock(&mutex);
            return 0;
        }
        //printf("end recv of %d\n", client);
        if (inputData->packetSubtype == ServiceUserAction && inputData->packetCode == GameRequestAction)
        {
            //boardSize = inputData->packetData[0];
            pthread_mutex_lock(&mutex);
            int idx = findNickname(USR, USRSZ, (inputData->packetData + 2));
            SOCKET opponent = USR[idx].usersock;
            char *inviteData = (char *) calloc(strlen(USR[curr].nickname) + 2, sizeof(char));
            pthread_mutex_unlock(&mutex);
            inviteData[0] = 3;
            pthread_mutex_lock(&mutex);
            inviteData[1] = (char) strlen(USR[curr].nickname);
            pthread_mutex_unlock(&mutex);
            for (int i = 0; i < inviteData[1]; i++)
            {
                pthread_mutex_lock(&mutex);
                inviteData[i + 2] = USR[curr].nickname[i];
                pthread_mutex_unlock(&mutex);
            }
            PACKET *otherInvite = initPacketFromParams(ServicePacket, ServiceUserAction, GameRequestAction, inviteData);
            char *rawInvite = encodePacket(otherInvite);
            pthread_mutex_lock(&mutex);
            printf("invite %s -> %s\n", USR[curr].nickname, USR[idx].nickname);
            pthread_mutex_unlock(&mutex);
            if (send(opponent, rawInvite, strlen(rawInvite), 0) == SOCKET_ERROR)
            {
                pthread_mutex_unlock(&mutex);
                printf("Error in sending smth to opponent_1\n%s\n", rawInvite);
                return 0;
            }
            pthread_mutex_lock(&mutex);
            USR[curr].role = CrossCell;
            USR[curr].opponentID = idx;
            USR[curr].game = initNewBoard(3);
            pthread_mutex_unlock(&mutex);
        }
        else if (inputData->packetSubtype == ServiceUserAction && inputData->packetCode == GameAcceptAction)
        {
            pthread_mutex_lock(&mutex);
            USR[curr].game = initNewBoard(3);
            USR[curr].role = ZeroCell;
            pthread_mutex_unlock(&mutex);
            for (int i = 0; i < USRSZ; i++)
            {
                if (USR[i].opponentID == curr)
                {
                    pthread_mutex_lock(&mutex);
                    USR[curr].opponentID = i;
                    pthread_mutex_unlock(&mutex);
                    break;
                }
            }
            pthread_mutex_lock(&mutex);
            printf("accept %s -> %s\n", USR[curr].nickname, USR[USR[curr].opponentID].nickname);
            pthread_mutex_unlock(&mutex);
            PACKET *ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
            char *okRaw = encodePacket(ok);
            pthread_mutex_lock(&mutex);
            if (send(USR[USR[curr].opponentID].usersock, okRaw, strlen(okRaw), 0) == SOCKET_ERROR)
            {
                pthread_mutex_unlock(&mutex);
                printf("Error in sending smth to opponent_2\n%s\n", okRaw);
                return 0;
            }
            printf("%s playing with %s\n", USR[curr].nickname, USR[USR[curr].opponentID].nickname);
            printf("curr is %d, opponent is %d", USR[curr].role, USR[USR[curr].opponentID].role);
            char *boardEncoding = encodeBoard(USR[curr].game);
            pthread_mutex_unlock(&mutex);
            PACKET *requestMove = initPacketFromParams(DataRequestPacket, RequestPlayerMove, 0, boardEncoding);
            char *requestMoveRaw = encodePacket(requestMove);
            pthread_mutex_lock(&mutex);
            if (send(USR[curr].usersock, requestMoveRaw, strlen(requestMoveRaw), 0) == SOCKET_ERROR)
            {
                pthread_mutex_unlock(&mutex);
                printf("Error in sending requestMove\n%s\n", requestMoveRaw);
                return 0;
            }
            printf("\nrequest move -> %s\n", USR[curr].nickname);
            pthread_mutex_unlock(&mutex);
        }
        else if (inputData->packetSubtype == ServiceUserAction && inputData->packetCode == GameDeclineAction)
        {
            printf("The opponent declined the offer\n");
            //Предложение отклонено... хахахах я думал ты сам напишешь
            PACKET* error = initPacketFromParams(ServicePacket, ServiceErrorPacket, GameDeclinedErr, 0);
            char* rerror = encodePacket(error);
            pthread_mutex_lock(&mutex);
            int oi = 0;
            for(int i = 0; i < USRSZ; i++) {
                if(USR[i].opponentID == curr) {
                    oi = i;
                    break;
                }
            }
            send(USR[oi].usersock, rerror, strlen(rerror), 0);
            pthread_mutex_unlock(&mutex);
            //return 0;
        }
        else if (inputData->packetSubtype == SendPlayerMove)
        {
            char x = inputData->packetData[0] - 1;
            char y = inputData->packetData[1] - 1;
            char *resend = encodePacket(inputData);
            //printf("DEBUG resend = %s\n", resend);
            pthread_mutex_lock(&mutex);
            printf("%s moving on %d, %d. Sending this to %s\n", USR[curr].nickname, x, y, USR[USR[curr].opponentID].nickname);
            if (send(USR[USR[curr].opponentID].usersock, resend, strlen(resend), 0) == SOCKET_ERROR)
            {
                pthread_mutex_unlock(&mutex);
                printf("Error in sending smth to opponent_3\n%s\n", resend);
                return 0;
            }
            pthread_mutex_unlock(&mutex);
            PACKET *ok = initPacketFromParams(ServicePacket, ServiceSuccess, 0, 0);
            char *okRaw = encodePacket(ok);
            pthread_mutex_lock(&mutex);
            if (send(USR[curr].usersock, okRaw, strlen(okRaw), 0) == SOCKET_ERROR)
            {
                pthread_mutex_unlock(&mutex);
                printf("Error in sending smth to opponent_4\n%s\n", okRaw);
                return 0;
            }
            makeMove(USR[curr].game, x, y, USR[curr].role);
            makeMove(USR[USR[curr].opponentID].game, x, y, USR[curr].role);
            char *boardEncoding = encodeBoard(USR[curr].game);
            pthread_mutex_unlock(&mutex);
            pthread_mutex_lock(&mutex);
            winner1 = checkBoardWinConditions(USR[curr].game);
            winner2 = checkBoardWinConditions(USR[USR[curr].opponentID].game);
            int draw = 0;
            if (winner1 == GAMEBOARD_NO_WIN && winner2 == GAMEBOARD_NO_WIN) {
                for(int i = 0; i < 9; i++) if (USR[curr].game->board[i] == EmptyCell) draw++;
            }
            //printf("draw = %d\n", draw);
            if (USR[curr].game && (winner1 != GAMEBOARD_NO_WIN && winner2 != GAMEBOARD_NO_WIN || !draw && winner1 == GAMEBOARD_NO_WIN)) {
                char* winner = NULL;
                int wlen = 0;
                if(!draw && winner1 != GAMEBOARD_NO_WIN) {
                    if (USR[curr].role == winner1) winner = USR[curr].nickname;
                    else winner = USR[USR[curr].opponentID].nickname;
                    wlen = strlen(winner);
                }
                pthread_mutex_unlock(&mutex);
                if(winner1 != GAMEBOARD_NO_WIN) printf("%s won!\n", winner);
                else printf("withdraw!\n");
                char* stopData = (char*)calloc(wlen+3, sizeof(char));
                stopData[0] = winner1 != GAMEBOARD_NO_WIN ? 2 : 3;
                stopData[1] = wlen;
                for(int i = 2; i < wlen + 2; i++) {
                    stopData[i] = winner[i - 2];
                }
                stopData[wlen+2] = 0;
                PACKET* endgame = initPacketFromParams(ServicePacket, ServiceEndGame, 0, stopData);
                char* antihype = encodePacket(endgame);
                //printf("winner data = %s\n", stopData);
                pthread_mutex_lock(&mutex);
                send(USR[curr].usersock, antihype, strlen(antihype), 0);
                send(USR[USR[curr].opponentID].usersock, antihype, strlen(antihype), 0);
                //printf("waiting for confirmation by ... %s and %s\n", USR[curr].nickname, USR[USR[curr].opponentID].nickname);
                USR[curr].game = 0;
                USR[USR[curr].opponentID].game = 0;
                USR[USR[curr].opponentID].opponentID = -1;
                USR[curr].opponentID = -1;
                pthread_mutex_unlock(&mutex);
                endgameconfirmationflag = 1;
                continue;
            }
            pthread_mutex_unlock(&mutex);
            PACKET *requestMove = initPacketFromParams(DataRequestPacket, RequestPlayerMove, 0, boardEncoding);
            char *requestMoveRaw = encodePacket(requestMove);
            pthread_mutex_lock(&mutex);
            if (send(USR[USR[curr].opponentID].usersock, requestMoveRaw, strlen(requestMoveRaw), 0) == SOCKET_ERROR)
            {
                pthread_mutex_unlock(&mutex);
                printf("Error in sending smth to opponent\n%s\n", requestMoveRaw);
                return 0;
            }
            pthread_mutex_unlock(&mutex);

        }
        else if(endgameconfirmationflag && inputData->packetSubtype == ServiceSuccess) {
            pthread_mutex_lock(&mutex);
            printf("ok received by %s\n", USR[curr].nickname);
            //send(USR[curr].usersock, "\xFF", 1, 0);
            pthread_mutex_unlock(&mutex);
            endgameconfirmationflag = -1;
            continue;
        }
    }
}

int main(int argc,char** argv)
{
    char logindata[2] = {1, 1};
    PACKET* sendLoginData = initPacketFromParams(DataSendPacket, SendPlayerMove, 0, logindata);
    char* enc = encodePacket(sendLoginData);
    pthread_mutex_init(&mutex, NULL);
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
        u_long mode = 1;
        ioctlsocket(client, FIONBIO, &mode);
        pthread_t thread;
        pthread_create(&thread, NULL, processRequest, (void *) &client);
        //pthread_join(thread, NULL);
        pthread_detach(thread);
        Sleep(100);
    }
    return 0;
}