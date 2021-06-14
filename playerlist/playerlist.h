#ifndef TICTACTOE_PLAYERLIST_H
#define TICTACTOE_PLAYERLIST_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "../userdata/userdata.h"

#define PLAYERLIST_NULL_PTR_ERR -1
#define PLAYERLIST_REALLOC_ERR -2

enum playerlistTypes {Top100List = 1, FriendsList = 2, BlackList = 3};

#define PLAYERLIST_START_ALLOCATED_LENGTH 64

typedef struct playerlist_struct
{
    short playerListLength;
    short allocatedLength;
    USER_DATA** playerdataList;
    char playerlistType;
}PLAYERLIST;

///@brief Init new playerlist from parameters
///@details Initializes new playerlist with provided paramters. playerdataList field is initialized to a pointer
///to USER_DATA* array of length playerListLength, and each element initialized to NULL
///@param playerlistLength Length of initialized playerlsit
///@param playerlistType Type of playerlist to initialize
///@retval NULL On error
///@retval PLAYERLIST* On success
PLAYERLIST* initPlayerlist(short playerlistLength,char playerlistType);

///@brief Frees specified player list
///@param freedPlayerlist Player list to free
void freePlayerlist(PLAYERLIST* freedPlayerlist);

///@brief Append new USER_DATA to list
///@param targetUserList List to append to
///@param appendedUserData User data that get appended.
/// NOTE: USER_DATA is appended by copy, original pointer can be modified without corrupting PLAYERLIST
///@param targetUserList User list to append to
///@param appendedUserData User data to append
///@retval 0 On success
///@retval PLAYERLIST_NULL_PTR_ERR NULL pointer encountered
///@retval PLAYERLIST_REALLOC_ERR reallocation failed
int appendToPlayerlist(PLAYERLIST* targetUserList, USER_DATA* appendedUserData);

///@brief Encodes top 100 player list for transmission
///@param encodedPlayerlist Top 100 list for encoding
///@retval NULL On error or when player list type is not Top100List
///@retval char* On success
char* encodeTop100Playerlist(PLAYERLIST* encodedPlayerlist);

///@brief Encodes top 100 player list for transmission
///@param encodedPlayerlist Top 100 list for encoding
///@retval NULL On error or when player list type is not FriendsList or BlackList
///@retval char* On success
char* encodeFriendAndBlacklist(PLAYERLIST* encodedPlayerlist);

///@brief Encodes user list for network transmission
///@param encodedPlayerList Player list to encode
///@retval NULL On error
///@retval char* On success
char* encodePlayerlist(PLAYERLIST* encodedPlayerList);

///@brief Decodes Top 100 playerlist
///@param encoding Received encoding
///@retval NULL On error
///@retval PLAYERLIST* On success
PLAYERLIST* decodeTop100Playerlist(char* encoding);

///@brief Decodes friend and black lists
///@param encoding Received encoding
///@retval NULL On error
///@retval PLAYERLIST* On success
PLAYERLIST* decodeFriendAndBlacklist(char* encoding);

///@brief Decodes playerlists
///@param encoding Received encoding
///@retval NULL On error
///@retval PLAYERLIST* On success
PLAYERLIST* decodePlayerlist(char* encoding);
#endif //TICTACTOE_PLAYERLIST_H
