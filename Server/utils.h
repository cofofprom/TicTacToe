#ifndef TICTACTOE_UTILS_H
#define TICTACTOE_UTILS_H

#include "..\clientNetworking\packet.h"
#include "..\clientNetworking\packetEnums.h"
#include "..\userdata\userdata.h"
#include <winsock2.h>

typedef struct {
    USER_DATA user;
    char* password;
    SOCKET usersock;
    int isAuth;
} SERVERUSER;

PACKET* buildPacket(char type, char subtype, char code);

int preLoginRoutine(SOCKET* s);

#endif //TICTACTOE_UTILS_H
