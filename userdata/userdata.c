#include "userdata.h"

USER_DATA *initUserData() {
    USER_DATA* newUserdata = calloc(1,sizeof(USER_DATA));
    if(newUserdata == NULL)
    {
        return NULL;
    }

    newUserdata->username = calloc(USERDATA_USERNAME_MAX_LENGTH + 1, sizeof(char));
    if(newUserdata->username == NULL)
    {
        free(newUserdata);
        return NULL;
    }

    newUserdata->gameCount = 0;
    newUserdata->lossCount = 0;
    newUserdata->userStatus = InvalidStatus;
    newUserdata->winCount = 0;

    return newUserdata;
}

USER_DATA *initNamedUserData(char *username) {
    if(username == NULL || strlen(username) > USERDATA_USERNAME_MAX_LENGTH)
    {
        return NULL;
    }

    USER_DATA* newUserData = initUserData();
    if(newUserData == NULL)
    {
        return NULL;
    }

    int result = memcpy_s(newUserData->username,USERDATA_USERNAME_MAX_LENGTH, username,strlen(username));
    if(result != 0)
    {
        free(newUserData->username);
        free(newUserData);

        return NULL;
    }

    newUserData->gameCount = 0;
    newUserData->lossCount = 0;
    newUserData->userStatus = InvalidStatus;
    newUserData->winCount = 0;

    return newUserData;
}

USER_DATA *copyUserdata(USER_DATA *copiedUserdata) {
    if(copiedUserdata == NULL)
    {
        return NULL;
    }

    USER_DATA* newUserData = initNamedUserData(copiedUserdata->username);

    newUserData->gameCount = copiedUserdata->gameCount;
    newUserData->lossCount = copiedUserdata->lossCount;
    newUserData->userStatus = copiedUserdata->userStatus;
    newUserData->winCount = copiedUserdata->winCount;

    return newUserData;
}

void freeUserData(USER_DATA *targetUserData) {
    if(targetUserData == NULL)
    {
        return;
    }

    if(targetUserData->username != NULL)
    {
        free(targetUserData->username);
    }

    free(targetUserData);

    return;
}

char *encodeUserData(USER_DATA *targetUserData) {
    if(targetUserData == NULL)
    {
        return NULL;
    }

    int encodingBufferLength = strlen(targetUserData->username) + 1;
    encodingBufferLength += sizeof(int32)*3;//Statistic coutners
    encodingBufferLength += 1;//Status byte

    char* outputBuffer = calloc(encodingBufferLength + 1, sizeof(char));
    if(outputBuffer == NULL)
    {
        return NULL;
    }

    char* currentBufferPos = outputBuffer;

    *currentBufferPos = strlen(targetUserData->username);
    currentBufferPos++;

    memcpy(currentBufferPos,targetUserData->username,strlen(targetUserData->username));
    currentBufferPos += strlen(targetUserData->username);

    memcpy(currentBufferPos,&targetUserData->winCount,sizeof(int32));
    currentBufferPos[0]++;//Prevent zeros in encoded string
    currentBufferPos[1]++;//Prevent zeros in encoded string
    currentBufferPos[2]++;//Prevent zeros in encoded string
    currentBufferPos[3]++;//Prevent zeros in encoded string
    currentBufferPos += sizeof(int32);

    memcpy(currentBufferPos,&targetUserData->lossCount,sizeof(int32));
    currentBufferPos[0]++;//Prevent zeros in encoded string
    currentBufferPos[1]++;//Prevent zeros in encoded string
    currentBufferPos[2]++;//Prevent zeros in encoded string
    currentBufferPos[3]++;//Prevent zeros in encoded string
    currentBufferPos += sizeof(int32);

    memcpy(currentBufferPos,&targetUserData->gameCount,sizeof(int32));
    currentBufferPos[0]++;//Prevent zeros in encoded string
    currentBufferPos[1]++;//Prevent zeros in encoded string
    currentBufferPos[2]++;//Prevent zeros in encoded string
    currentBufferPos[3]++;//Prevent zeros in encoded string
    currentBufferPos += sizeof(int32);

    *currentBufferPos = targetUserData->userStatus;

    return outputBuffer;

}

USER_DATA *decodeUserData(char *encoding) {
    if(encoding == NULL)
    {
        return NULL;
    }

    USER_DATA* newUserData = initUserData();
    if(newUserData == NULL)
    {
        return NULL;
    }

    char* currentBufferPos = encoding;

    char nicknameLength = currentBufferPos[0];
    currentBufferPos++;

    memcpy(newUserData->username,currentBufferPos,nicknameLength);
    currentBufferPos += nicknameLength;

    currentBufferPos[0]--;//Revert zero prevention
    currentBufferPos[1]--;//Revert zero prevention
    currentBufferPos[2]--;//Revert zero prevention
    currentBufferPos[3]--;//Revert zero prevention
    memcpy(&newUserData->winCount,currentBufferPos,sizeof(int32));
    currentBufferPos += sizeof(int32);

    currentBufferPos[0]--;//Revert zero prevention
    currentBufferPos[1]--;//Revert zero prevention
    currentBufferPos[2]--;//Revert zero prevention
    currentBufferPos[3]--;//Revert zero prevention
    memcpy(&newUserData->lossCount,currentBufferPos,sizeof(int32));
    currentBufferPos += sizeof(int32);

    currentBufferPos[0]--;//Revert zero prevention
    currentBufferPos[1]--;//Revert zero prevention
    currentBufferPos[2]--;//Revert zero prevention
    currentBufferPos[3]--;//Revert zero prevention
    memcpy(&newUserData->gameCount,currentBufferPos,sizeof(int32));
    currentBufferPos += sizeof(int32);

    newUserData->userStatus = *currentBufferPos;

    return newUserData;
}
