#ifndef TICTACTOE_PACKET_H
#define TICTACTOE_PACKET_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "packetEnums.h"

typedef struct packet_struct
{
    short packetLength;
    char packetType;
    char packetSubtype;
    char packetCode;
    char* packetData;
}PACKET;

///@brief Initialize new packet struct
///@warning packetData field remains uninitialized NULL pointer
///@retval NULL On error
///@retval PACKET* On success
PACKET* initPacket();

///@brief Initialize new packet from parameters
///@details Initializes new packet. If packet subtype requires packet code, it is set
///@warning This function does not check for packet validity
///@param packetType Packet type to construct
///@param packetSubtype Packet subtype to construct
///@param packetCode Packet code to construct
///@param data pointer to data to include, if NULL, no data is appended
///@retval NULL On error
///@retval PACKET* On success
PACKET* initPacketFromParams(char packetType, char packetSubtype, char packetCode, char* data);

///@brief Decode packet from received data stream
///@param encoding Encoded packet. Received packet data terminated with 0, including first two bytes encoding packet length;
///@retval NULL On error
///@retval PACKET* On success
PACKET* decodePacket(char* encoding);

///@brief Encode packet for network transmission
///@param targetPacket Packet to encode
///@retval NULL On error
///@retval char* Packet encoding. Zero-terminated string ready for network transmission.
char* encodePacket(PACKET* targetPacket);

///@brief Free packet struct
///@param targetPacket Packet struct to be freed
void freePacket(PACKET* targetPacket);

void printPacketDebug(PACKET* targetPacket);

#endif //TICTACTOE_PACKET_H
