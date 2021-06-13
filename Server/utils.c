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