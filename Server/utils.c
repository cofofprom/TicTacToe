#include "utils.h"

PACKET* buildPacket(char type, char subtype, char code) {
    PACKET* target = initPacket();
    target->packetType = type;
    target->packetSubtype = subtype;
    target->packetCode = code;
}

int preLoginRoutine(SOCKET* client) {
    PACKET* loginRequest = initPacket();
    loginRequest->packetType = DataRequestPacket;
    loginRequest->packetSubtype = LoginRequest;
    loginRequest->packetCode = 0;
    loginRequest->packetData = 0;
    loginRequest->packetLength = 5;
    char* rawPacket = encodePacket(loginRequest);
    freePacket(loginRequest);
    return send(*client, rawPacket, (int)strlen(rawPacket), 0);
}

char* serialize(SERVERUSER_LITE* user) {
    int nicksize = strlen(user->nickname);
    int passsize = strlen(user->password);
    char* data = (char*)calloc(nicksize + passsize + 2, 1);
    data[0] = (char)nicksize;
    for(int i = 0; i < nicksize; i++) {
        data[i + 1] = user->nickname[i];
    }
    for(int i = 0; i < passsize; i++) {
        data[i + nicksize + 2] = user->password[i];
    }
    return data;
}