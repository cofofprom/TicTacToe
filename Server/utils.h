#ifndef TICTACTOE_UTILS_H
#define TICTACTOE_UTILS_H

#include "..\clientNetworking\packet.h"
#include "..\clientNetworking\packetEnums.h"
#include "..\userdata\userdata.h"
#include <string.h>
#include <winsock2.h>
#include "..\gameboard\gameboard.h"

typedef struct {
    USER_DATA user;
    char* password;
    SOCKET usersock;
    int isAuth;

} SERVERUSER;

typedef struct {
    char* nickname;
    SOCKET usersock;
    int role;
    int opponentID;
    GAME_BOARD* game
} SERVERUSER_LITE;

SERVERUSER_LITE* initUser(char* nickname, SOCKET usersock);

char* serialize(SERVERUSER_LITE* user);
SERVERUSER_LITE* deserialize(char* user);

int preLoginRoutine(SOCKET* s);

int checkNickname(SERVERUSER_LITE* arr, int sz, const char* nickname);

int findNickname(SERVERUSER_LITE* arr, int sz, const char* nick);

#endif //TICTACTOE_UTILS_H
