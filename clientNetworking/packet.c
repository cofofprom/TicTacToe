#include "packet.h"


PACKET *initPacket() {
    PACKET* newPacket = calloc(1, sizeof(PACKET));
    if(newPacket == NULL)
    {
        return NULL;
    }

    newPacket->packetCode = -1;
    newPacket->packetData = NULL;
    newPacket->packetSubtype = -1;
    newPacket->packetType = -1;
    newPacket->packetLength = -1;

    return newPacket;
}

PACKET *decodePacket(char *encoding) {
    if(encoding == NULL)
    {
        return NULL;
    }

    PACKET* newPacket = initPacket();
    if(newPacket == NULL)
    {
        return NULL;
    }

    char* currentEncodingPos = encoding;

    memcpy(&newPacket->packetLength,currentEncodingPos,sizeof(short));
    currentEncodingPos += sizeof(short);

    newPacket->packetType = *currentEncodingPos;
    currentEncodingPos++;

    newPacket->packetSubtype = *currentEncodingPos;
    currentEncodingPos++;

    if(newPacket->packetSubtype == ErrorPacket ||
    newPacket->packetSubtype == ServiceUserAction ||
    newPacket->packetSubtype == RequestPlayerList ||
    newPacket->packetSubtype == ServiceNotificationPacket)
    {
        newPacket->packetCode = *currentEncodingPos;
        currentEncodingPos++;
    }

    int expectedPacketDataLength = newPacket->packetLength - 2;
    if(newPacket->packetCode != -1)
    {
        expectedPacketDataLength -= 1;
    }

    newPacket->packetData = calloc(expectedPacketDataLength + 1, sizeof(char));
    if(newPacket->packetData == NULL)
    {
        free(newPacket);
        return NULL;
    }

    memcpy(newPacket->packetData,currentEncodingPos,expectedPacketDataLength);

    return newPacket;

}

char *encodePacket(PACKET *targetPacket) {
    if(targetPacket == NULL)
    {
        return NULL;
    }

    char* encoding = calloc(targetPacket->packetLength+1, sizeof(char));
    if(encoding == NULL)
    {
        return NULL;
    }

    char* currentEncodingPos = encoding;

    memcpy(currentEncodingPos,&targetPacket->packetLength,sizeof(short));

    currentEncodingPos += sizeof(short);

    *currentEncodingPos = targetPacket->packetType;
    currentEncodingPos++;

    *currentEncodingPos = targetPacket->packetSubtype;
    currentEncodingPos++;

    if(targetPacket->packetSubtype == ErrorPacket ||
       targetPacket->packetSubtype == ServiceUserAction ||
       targetPacket->packetSubtype == RequestPlayerList ||
       targetPacket->packetSubtype == ServiceNotificationPacket)
    {
        *currentEncodingPos = targetPacket->packetCode;
        currentEncodingPos++;
    }

    int expectedPacketDataLength = targetPacket->packetLength - 2;
    if(targetPacket->packetCode != -1)
    {
        expectedPacketDataLength -= 1;
    }
    int dataLength = strlen(targetPacket->packetData);

    if(dataLength != expectedPacketDataLength)
    {
        free(encoding);
        return NULL;
    }

    memcpy(currentEncodingPos, targetPacket->packetData, expectedPacketDataLength);

    currentEncodingPos += expectedPacketDataLength;

    return encoding;
}

void freePacket(PACKET *targetPacket) {
    if(targetPacket == NULL)
    {
        return;
    }
    if(targetPacket->packetData != NULL)
    {
        free(targetPacket->packetData);
    }

    free(targetPacket);

    return;
}
