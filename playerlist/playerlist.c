#include "playerlist.h"

PLAYERLIST *initPlayerlist(short playerlistLength,char playerlistType) {
    if(playerlistType < 1 || playerlistType > 3)
    {
        return NULL;
    }
    PLAYERLIST* newPlayerlist = calloc(1, sizeof(PLAYERLIST));
    if(newPlayerlist == NULL)
    {
        return NULL;
    }

    newPlayerlist->playerdataList = calloc(playerlistLength,sizeof(USER_DATA*));
    if(newPlayerlist->playerdataList == NULL)
    {
        free(newPlayerlist);
        return  NULL;
    }

    newPlayerlist->playerListLength = 0;
    newPlayerlist->allocatedLength = playerlistLength;

    return newPlayerlist;
}

void freePlayerlist(PLAYERLIST *freedPlayerlist) {
    if(freedPlayerlist == NULL)
    {
        return;
    }

    if(freedPlayerlist->playerdataList)
    {
        free(freedPlayerlist);
        return;
    }

    for(int i = 0; i < freedPlayerlist->playerListLength; i++)
    {
        if(freedPlayerlist->playerdataList[i] != NULL)
        {
            freeUserData(freedPlayerlist->playerdataList[i]);
        }
    }

    free(freedPlayerlist->playerdataList);
    free(freedPlayerlist);

    return;
}

char *encodeTop100Playerlist(PLAYERLIST *encodedPlayerlist) {
    if(encodedPlayerlist == NULL || encodedPlayerlist->playerdataList == NULL)
    {
        return NULL;
    }

    int encodingLength = 2;
    for(int i = 0; i < encodedPlayerlist->playerListLength; i++)
    {
        if(encodedPlayerlist->playerdataList[i] != NULL)
        {
            encodingLength += strlen(encodedPlayerlist->playerdataList[i]->username)+1+4+4;//name + length + 2 coutners
        }
    }

    char* encoding = calloc(encodingLength + 1, sizeof(char));
    if(encoding == NULL)
    {
        return NULL;
    }

    char* currentPosInEncoding = encoding;
    memcpy(currentPosInEncoding,&encodedPlayerlist->playerListLength,sizeof(short));
    currentPosInEncoding[0] += 1;//Zero avoidance
    currentPosInEncoding[1] += 1;//Zero avoidance
    currentPosInEncoding += sizeof(short);

    for(int i = 0; i < encodedPlayerlist->playerListLength; i++)
    {
        if(encodedPlayerlist->playerdataList[i] != NULL)
        {
            char usernameLength = strlen(encodedPlayerlist->playerdataList[i]->username);
            *currentPosInEncoding = usernameLength;
            currentPosInEncoding++;

            memcpy(currentPosInEncoding,encodedPlayerlist->playerdataList[i]->username,usernameLength);
            currentPosInEncoding+= usernameLength;

            memcpy(currentPosInEncoding,&encodedPlayerlist->playerdataList[i]->winCount,sizeof(int32));
            currentPosInEncoding[0]++;//Zero avoidance
            currentPosInEncoding[1]++;//Zero avoidance
            currentPosInEncoding[2]++;//Zero avoidance
            currentPosInEncoding[3]++;//Zero avoidance
            currentPosInEncoding += sizeof(int32);

            memcpy(currentPosInEncoding,&encodedPlayerlist->playerdataList[i]->lossCount,sizeof(int32));
            currentPosInEncoding[0]++;//Zero avoidance
            currentPosInEncoding[1]++;//Zero avoidance
            currentPosInEncoding[2]++;//Zero avoidance
            currentPosInEncoding[3]++;//Zero avoidance
            currentPosInEncoding += sizeof(int32);
        }
    }

    return encoding;
}

char *encodeFriendAndBlacklist(PLAYERLIST *encodedPlayerlist) {
    if(encodedPlayerlist == NULL || encodedPlayerlist->playerdataList == NULL)
    {
        return NULL;
    }

    int encodingLength = 2;
    for(int i = 0; i < encodedPlayerlist->playerListLength; i++)
    {
        if(encodedPlayerlist->playerdataList[i] != NULL)
        {
            encodingLength += strlen(encodedPlayerlist->playerdataList[i]->username)+1+2+2;//name + length + 2 coutners
        }
    }

    char* encoding = calloc(encodingLength + 1, sizeof(char));
    if(encoding == NULL)
    {
        return NULL;
    }

    char* currentPosInEncoding = encoding;
    memcpy(currentPosInEncoding,&encodedPlayerlist->playerListLength,sizeof(short));
    currentPosInEncoding[0] += 1;//Zero avoidance
    currentPosInEncoding[1] += 1;//Zero avoidance
    currentPosInEncoding += sizeof(short);

    for(int i = 0; i < encodedPlayerlist->playerListLength; i++)
    {
        if(encodedPlayerlist->playerdataList[i] != NULL)
        {
            char usernameLength = strlen(encodedPlayerlist->playerdataList[i]->username);
            *currentPosInEncoding = usernameLength;
            currentPosInEncoding++;

            memcpy(currentPosInEncoding,encodedPlayerlist->playerdataList[i]->username,usernameLength);
            currentPosInEncoding+= usernameLength;
        }
    }

    return encoding;
}

char *encodePlayerlist(PLAYERLIST *encodedPlayerList) {
    if(encodedPlayerList == NULL || encodedPlayerList->playerdataList == NULL)
    {
        return NULL;
    }

    char* encoding = NULL;

    switch (encodedPlayerList->playerlistType)
    {
        case Top100List:
            encoding = encodeTop100Playerlist(encodedPlayerList);
            if(encoding == NULL)
            {
                return NULL;
            }
            break;

            case FriendsList:
            case BlackList:
                encoding = encodeFriendAndBlacklist(encodedPlayerList);
                if(encoding == NULL)
                {
                    return NULL;
                }
                break;
    }

    return encoding;
}

int appendToPlayerlist(PLAYERLIST *targetUserList, USER_DATA *appendedUserData) {
    if(targetUserList == NULL || appendedUserData == NULL || targetUserList->playerdataList == NULL)
    {
        PLAYERLIST_NULL_PTR_ERR;
    }

    if(targetUserList->allocatedLength == targetUserList->playerListLength)
    {
        USER_DATA** newList = realloc(targetUserList->playerdataList,sizeof(USER_DATA*)*targetUserList->allocatedLength*2);
        if(newList == NULL)
        {
            return PLAYERLIST_REALLOC_ERR;
        }
        targetUserList->playerdataList = newList;
        targetUserList->allocatedLength *= 2;
    }
    targetUserList->playerdataList[targetUserList->playerListLength] = copyUserdata(appendedUserData);
    targetUserList->playerListLength++;

    return 0;
}

PLAYERLIST *decodeTop100Playerlist(char *encoding) {
    if(encoding == NULL)
    {
        return NULL;
    }

    char* currentPosInEncoding = encoding;

    short playerlistLength = 0;
    currentPosInEncoding[0]--;
    currentPosInEncoding[1]--;
    memcpy(&playerlistLength,currentPosInEncoding,sizeof(short));
    currentPosInEncoding += sizeof(short);

    PLAYERLIST* newPlayerlist = initPlayerlist(playerlistLength,Top100List);

    for(int i = 0; i < playerlistLength; i++)
    {
        char playerNameLength = *currentPosInEncoding;
        currentPosInEncoding++;
        char* username = calloc(playerNameLength + 1, sizeof(char));
        memcpy(username,currentPosInEncoding,playerNameLength);
        currentPosInEncoding += playerNameLength;

        USER_DATA* newUserdata = initNamedUserData(username);


        currentPosInEncoding[0]--;//Zero avoidance undo
        currentPosInEncoding[1]--;//Zero avoidance undo
        currentPosInEncoding[2]--;//Zero avoidance undo
        currentPosInEncoding[3]--;//Zero avoidance undo
        memcpy(&newUserdata->winCount,currentPosInEncoding,sizeof(int32));
        currentPosInEncoding += sizeof(int32);

        currentPosInEncoding[0]--;//Zero avoidance undo
        currentPosInEncoding[1]--;//Zero avoidance undo
        currentPosInEncoding[2]--;//Zero avoidance undo
        currentPosInEncoding[3]--;//Zero avoidance undo
        memcpy(&newUserdata->lossCount,currentPosInEncoding,sizeof(int32));
        currentPosInEncoding += sizeof(int32);

        appendToPlayerlist(newPlayerlist,newUserdata);

        freeUserData(newUserdata);
    }

    return newPlayerlist;
}

PLAYERLIST *decodeFriendAndBlacklist(char *encoding) {
    if(encoding == NULL)
    {
        return NULL;
    }

    char* currentPosInEncoding = encoding;

    short playerlistLength = 0;
    currentPosInEncoding[0]--;
    currentPosInEncoding[1]--;
    memcpy(&playerlistLength,currentPosInEncoding,sizeof(short));
    currentPosInEncoding += sizeof(short);

    PLAYERLIST* newPlayerlist = initPlayerlist(playerlistLength,Top100List);

    for(int i = 0; i < playerlistLength; i++)
    {
        char playerNameLength = *currentPosInEncoding;
        currentPosInEncoding++;
        char* username = calloc(playerNameLength + 1, sizeof(char));
        memcpy(username,currentPosInEncoding,playerNameLength);
        currentPosInEncoding += playerNameLength;

        USER_DATA* newUserdata = initNamedUserData(username);

        appendToPlayerlist(newPlayerlist,newUserdata);

        freeUserData(newUserdata);
    }

    return newPlayerlist;
}
