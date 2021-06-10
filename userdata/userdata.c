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
