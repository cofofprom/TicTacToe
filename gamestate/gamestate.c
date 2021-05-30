#include "gamestate.h"

GAME_STATE_HISTORY *initGameStateHistory(char boardSize) {
    if(boardSize > MAX_BOARD_SIZE || boardSize < MIN_BOARD_SIZE)
    {
        return NULL;
    }

    GAME_STATE_HISTORY* newHistory = calloc(1, sizeof(GAME_STATE_HISTORY));
    if(newHistory == NULL)
    {
        return NULL;
    }

    newHistory->boards = calloc(boardSize*boardSize + 1,sizeof(char*));
    if(newHistory->boards == NULL)
    {
        free(newHistory);
        return NULL;
    }

    newHistory->allocatedSize = boardSize*boardSize + 1;
    newHistory->boardSize = boardSize;
    newHistory->currentStep = -1;
    newHistory->iteratorIndex = -1;
    newHistory->steps = 0;

    return newHistory;
}

void freeGameStateHistory(GAME_STATE_HISTORY *targetGameHistory) {
    if(targetGameHistory == NULL)
    {
        return;
    }

    if(targetGameHistory->boards == NULL)
    {
        return;
    }

    for(int i = 0; i < targetGameHistory->allocatedSize; i++)
    {
        if(targetGameHistory->boards[i] != 0)
        {
            free(targetGameHistory->boards[i]);
        }
    }

    free(targetGameHistory->boards);

    free(targetGameHistory);

    return;
}
