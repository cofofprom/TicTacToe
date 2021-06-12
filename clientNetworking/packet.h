#ifndef TICTACTOE_PACKET_H
#define TICTACTOE_PACKET_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

enum packetType { ServicePacket = 1, DataRequestPacket = 2, dataSendPacket = 3};
enum servicePacketType {PingRequest = 1, PingResponse = 2, ErrorPacket = 3, Success = 4, UserAction = 5, EndGame = 6, NotificationPacket = 7};
enum dataRequestPacketType {LoginRequest = 1, RequestProfileData = 2, RequestPlayerList = 3,
    RequestGameHistory = 4, RequestPlayerMove = 5, CheckUsername = 6, CheckUserStatus = 7};
enum dataSendPacketType {LoginData = 1, RegisterRequest = 2, ProfileData = 3, PlayerList = 4,
    GameStateHistory = 5, PlayerMove = 6, UserStatus = 7};

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
