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

#endif //TICTACTOE_PACKET_H
