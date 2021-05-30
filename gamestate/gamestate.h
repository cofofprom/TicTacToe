#ifndef TICTACTOE_GAMESTATE_H
#define TICTACTOE_GAMESTATE_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "../gameboard/gameboard.h"

typedef struct gamestate_history_struct
{
    int steps;
    char boardSize;
    int allocatedSize;
    char** boards;
} GAME_STATE_HISTORY;

GAME_STATE_HISTORY* initGameStateHistory();

void freeGameStateHistory(GAME_STATE_HISTORY* targetGameHistory);

int appendBoardState(GAME_STATE_HISTORY* target, GAME_BOARD* board);

GAME_BOARD* getBoardFromState(GAME_STATE_HISTORY* targetHistory, int index);

int updateBoardFromState(GAME_STATE_HISTORY* targetHistory, GAME_BOARD* targetBoard, int index);

#endif //TICTACTOE_GAMESTATE_H
