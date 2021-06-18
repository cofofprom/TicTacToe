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

PACKET *initPacketFromParams(char packetType, char packetSubtype, char packetCode, char *data) {
    PACKET* newPacket = initPacket();
    if(newPacket == NULL)
    {
        return NULL;
    }

    newPacket->packetLength = 2;
    newPacket->packetType = packetType;
    newPacket->packetSubtype = packetSubtype;
    if(newPacket->packetType == ServicePacket && newPacket->packetSubtype == ServiceErrorPacket ||
       newPacket->packetType == ServicePacket && newPacket->packetSubtype == ServiceUserAction ||
       newPacket->packetType == DataRequestPacket && newPacket->packetSubtype == RequestPlayerList ||
       newPacket->packetType == ServicePacket && newPacket->packetSubtype == ServiceNotificationPacket)
    {
        newPacket->packetCode = packetCode;
        newPacket->packetLength++;
    } else{
        newPacket->packetCode = -1;
    }

    if(data != NULL)
    {
        short dataLength = strlen(data);
        newPacket->packetData = calloc(dataLength + 1, sizeof(char));
        memcpy(newPacket->packetData,data,dataLength);
        newPacket->packetLength += dataLength;
    }

    return newPacket;
}

PACKET *makePacketCopy(PACKET *copiedPacket) {
    if(copiedPacket == NULL)
    {
        return NULL;
    }

    PACKET* newPacket = initPacketFromParams(copiedPacket->packetType,copiedPacket->packetSubtype,copiedPacket->packetCode,copiedPacket->packetData);
    if(newPacket == NULL)
    {
        return NULL;
    }

    return newPacket;
}

PACKET *decodePacket(char *encoding) {
    if (encoding == NULL) {
        return NULL;
    }

    PACKET *newPacket = initPacket();
    if (newPacket == NULL) {
        return NULL;
    }

    char *currentEncodingPos = encoding;

    currentEncodingPos[0]--;
    currentEncodingPos[1]--;
    memcpy(&newPacket->packetLength, currentEncodingPos, sizeof(short));
    currentEncodingPos += sizeof(short);

    newPacket->packetType = *currentEncodingPos;
    currentEncodingPos++;

    newPacket->packetSubtype = *currentEncodingPos;
    currentEncodingPos++;

    if (newPacket->packetType == ServicePacket && newPacket->packetSubtype == ServiceErrorPacket ||
        newPacket->packetType == ServicePacket && newPacket->packetSubtype == ServiceUserAction ||
        newPacket->packetType == DataRequestPacket && newPacket->packetSubtype == RequestPlayerList ||
        newPacket->packetType == ServicePacket && newPacket->packetSubtype == ServiceNotificationPacket) {
        newPacket->packetCode = *currentEncodingPos;
        currentEncodingPos++;
    }

    int expectedPacketDataLength = newPacket->packetLength - 2;
    if (newPacket->packetCode != -1) {
        expectedPacketDataLength -= 1;
    }

    if (expectedPacketDataLength != 0)
    {
            newPacket->packetData = calloc(expectedPacketDataLength + 1, sizeof(char));
        if (newPacket->packetData == NULL) {
            free(newPacket);
            return NULL;
        }

        memcpy(newPacket->packetData, currentEncodingPos, expectedPacketDataLength);
    }

    return newPacket;
}

char *encodePacket(PACKET *targetPacket) {
    if(targetPacket == NULL)
    {
        return NULL;
    }

    char* encoding = calloc(targetPacket->packetLength+2+1, sizeof(char));
    if(encoding == NULL)
    {
        return NULL;
    }

    char* currentEncodingPos = encoding;

    memcpy(currentEncodingPos,&targetPacket->packetLength,sizeof(short));
    currentEncodingPos[0]++;
    currentEncodingPos[1]++;
    currentEncodingPos += sizeof(short);

    *currentEncodingPos = targetPacket->packetType;
    currentEncodingPos++;

    *currentEncodingPos = targetPacket->packetSubtype;
    currentEncodingPos++;

    if(targetPacket->packetType == ServicePacket && targetPacket->packetSubtype == ServiceErrorPacket ||
       targetPacket->packetType == ServicePacket && targetPacket->packetSubtype == ServiceUserAction ||
       targetPacket->packetType == DataRequestPacket && targetPacket->packetSubtype == RequestPlayerList ||
       targetPacket->packetType == ServicePacket && targetPacket->packetSubtype == ServiceNotificationPacket)
    {
        *currentEncodingPos = targetPacket->packetCode;
        currentEncodingPos++;
    }

    if(targetPacket->packetData != NULL)
    {
        int expectedPacketDataLength = targetPacket->packetLength - 2;
        if (targetPacket->packetCode != -1) {
            expectedPacketDataLength -= 1;
        }
        int dataLength = strlen(targetPacket->packetData);

        if (dataLength != expectedPacketDataLength) {
            free(encoding);
            return NULL;
        }

        memcpy(currentEncodingPos, targetPacket->packetData, expectedPacketDataLength);

        currentEncodingPos += expectedPacketDataLength;
    }

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

void printPacketDebug(PACKET *targetPacket) {
    if(targetPacket == NULL)
    {
        return;
    }
    printf("-----------------------------\n");
    printf("Packet type: %d\n", targetPacket->packetType);
    printf("Packet subtype: %d\n", targetPacket->packetSubtype);
    printf("Packet code: %d\n", targetPacket->packetCode);
    if(targetPacket->packetData !=  NULL)
    {
        printf("Packet data: %s\n",targetPacket->packetData);
    } else{
        printf("Packet data: None\n");
    }
    printf("-----------------------------\n");
}
