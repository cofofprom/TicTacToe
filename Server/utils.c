#include "utils.h"

int preLoginRoutine(SOCKET* client) {
    PACKET* loginRequest = initPacketFromParams(DataRequestPacket, LoginRequest, 0, 0);
    char* rawPacket = encodePacket(loginRequest);
    freePacket(loginRequest);
    int retval = send(*client, rawPacket, (int)strlen(rawPacket), 0);
    //printf("ERROR IS %d", WSAGetLastError());
    return retval;
}

char* serialize(SERVERUSER_LITE* user) {
    int nicksize = (int)strlen(user->nickname);
    char* data = (char*)calloc(nicksize + 1, 1);
    data[0] = (char)nicksize;
    for(int i = 0; i < nicksize; i++) {
        data[i + 1] = user->nickname[i];
    }
    data[nicksize + 2] = 0;
    return data;
}

SERVERUSER_LITE* deserialize(char* user) {
    SERVERUSER_LITE* usr = (SERVERUSER_LITE*)calloc(1, sizeof(SERVERUSER_LITE));
    char nicksize = user[0];
    usr->nickname = (char*)calloc(nicksize + 1, 1);
    for(int i = 0; i < nicksize; i++) {
        usr->nickname[i] = user[i + 1];
    }
    usr->nickname[nicksize + 1] = 0;
    usr->role = 0;
    usr->usersock = 0;
    return usr;
}

int checkNickname(SERVERUSER_LITE* arr, int sz, const char* nickname) {
    char loginLen = nickname[0];
    char* login = (char*)calloc(loginLen + 1, sizeof(char));
    for(int i = 0; i < loginLen; i++) {
        login[i] = nickname[i + 1];
    }
    int okflag = 1;
    for(int i = 0; i < sz; i++) {
        if(!strcmp(arr[i].nickname, login)) {
            //PACKET* err = initPacketFromParams(ServicePacket, ServiceErrorPacket, NoRegUsernameTakenErr, 0);
            //char* raw = encodePacket(err);
            //send(*client, raw, (int)strlen(raw), 5);
            okflag = 0;
            break;
        }
    }
    return okflag;
}

int findNickname(SERVERUSER_LITE* arr, int sz, const char* nick) {
    for(int i = 0; i < sz; i++) {
        if(!strcmp(arr[i].nickname, nick) && arr[i].online) return i;
    }
    return -1;
}

SERVERUSER_LITE* initUser(char* nickname, SOCKET usersock) {
    SERVERUSER_LITE* target = (SERVERUSER_LITE*)calloc(1, sizeof(SERVERUSER_LITE));
    target->usersock = usersock;
    target->nickname = nickname;
    target->role = 0;
    target->opponentID = -1;
    target->online = 1;
}

int recvPacket(SOCKET s, PACKET** p) {
    short packetLen;
    char* packet = NULL;
    char* packetLenArr = (char*)calloc(3, sizeof(char));
    int waitingForPacket = 1;
    do {
        int bytes = recv(s, packetLenArr, 2, MSG_PEEK);
        if (bytes == 2) {
            recv(s, packetLenArr, 2, 0);
            packetLenArr[0]--;
            packetLenArr[1]--;
            memcpy(&packetLen, packetLenArr, sizeof(short));
            packet = (char*)calloc(packetLen + 3, sizeof(char));
        } else if (bytes == SOCKET_ERROR) {
            int error = WSAGetLastError();
            //printf("ERROR %d\n", error);
            if(error != WSAEWOULDBLOCK)
                return SOCKET_ERROR;
        }
    } while(packet == NULL);
    while(waitingForPacket) {
        //printf("Waiting for full\n");
        int bytes = recv(s, packet, packetLen, MSG_PEEK);
        if(bytes == packetLen) {
            recv(s, packet + 2, packetLen, 0);
            memcpy(packet, &packetLen, sizeof(short));
            packet[0]++;
            packet[1]++;
            waitingForPacket = 0;
        } else if (bytes == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) return SOCKET_ERROR;
    }
    *p = decodePacket(packet);
    //if(strlen(packet)==1) printf("DEBUG = %s\n", packet);
    //printf("DEBUG rawpacket = %s\n", packet);
    return strlen(packet);
}