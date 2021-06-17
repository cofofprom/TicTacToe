#ifndef TICTACTOE_SERVERUSERDATA_H
#define TICTACTOE_SERVERUSERDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include "../userdata/userdata.h"

#define USERDATA_USERNAME_MAX_LENGTH 32

#define USERDATA_SUCCESS 0
#define USERDATA_NULL_PTR -1
#define USERDATA_USERNAME_TOO_LONG -2

typedef int32_t int32;

typedef struct server_userdata_struct
{
    char* username;
    char* password;
    int32 winCount;
    int32 lossCount;
    int32 gameCount;
    char userStatus;
}SERVER_USER_DATA;

typedef struct server_userbase_struct
{
    SERVER_USER_DATA* database;
    int dbLength
}SERVER_USERBASE;

SERVER_USER_DATA* initServerUsrdata(char* username, char* password, int32 winCount, int32 lossCount, int32 gameCount, char userStatus);

void freeSrvUserData(SERVER_USER_DATA* targetUserData);

char* serializeSrvUserData(SERVER_USER_DATA* targetUserData);

SERVER_USER_DATA * deserializeSrvUserData(SERVER_USER_DATA* targetUserData);



SERVER_USERBASE* initUserbase(int length);

void freeUserbase(SERVER_USERBASE* targetUserbase);

char* serializeUserbase(SERVER_USERBASE* targetUserbase);

SERVER_USERBASE* deserializeUserbase(SERVER_USERBASE* targetUserbase);

void serverUserbaseToFile(SERVER_USERBASE* targetUserbase, char* filepath);

SERVER_USERBASE* userbaseFromFile(char* filepath);

#endif //TICTACTOE_SERVERUSERDATA_H
