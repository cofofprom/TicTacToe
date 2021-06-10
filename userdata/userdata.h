#ifndef TICTACTOE_USERDATA_H
#define TICTACTOE_USERDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>

#define USERDATA_USERNAME_MAX_LENGTH 32

#define USERDATA_SUCCESS 0
#define USERDATA_NULL_PTR -1
#define USERDATA_USERNAME_TOO_LONG -2

typedef int32_t int32;

typedef enum user_status_enum{Offline = 1,OnlineNogame = 2,OnlineIngame = 3, InvalidStatus = 4} USER_STATUS;

typedef struct userdata_struct
{
    char* username;
    int32 winCount;
    int32 lossCount;
    int32 gameCount;
    char userStatus;
}USER_DATA;

///@brief Initialize new user data struct
///@details Initializes user data struct without a username
///@warning Username field is still allocated with USERDATA_USERNAME_MAX_LENGTH + 1 bytes set to zero
///@retval NULL On error
///@retval USER_DATA* On success
USER_DATA* initUserData();

///@brief Initialize new user data struct with set username
///@details Initializes new user data with provided username
///@param username Username to initialize new userdata with
///@retval NULL On error
///@retval USER_DATA* On success
USER_DATA* initNamedUserData(char* username);

///@brief Dispose of userdata structure
///@param targetUserData Userdata to dispose of
void freeUserData(USER_DATA* targetUserData);

///@brief Encode user data to null-terminated string
///@param targetUserData User data to encode
///@retval NULL On error
///@retval char* Userdata encoding
char* encodeUserData(USER_DATA* targetUserData);

///@brief Decode user data
///@param encoding User data encoding
///@retval NULL On error
///@retval USER_DATA* Decoded user data
USER_DATA* decodeUserData(char* encoding);

#endif //TICTACTOE_USERDATA_H
