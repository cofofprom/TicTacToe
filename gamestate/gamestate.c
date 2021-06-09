#include "gamestate.h"

GAME_STATE_HISTORY *initGameStateHistory(char boardSize, char* player1name, char* player2name) {
    if(boardSize > MAX_BOARD_SIZE || boardSize < MIN_BOARD_SIZE)
    {
        return NULL;
    }

    if(player1name == NULL || player2name == NULL)
    {
        return NULL;
    }

    GAME_STATE_HISTORY* newHistory = calloc(1, sizeof(GAME_STATE_HISTORY));
    if(newHistory == NULL)
    {
        return NULL;
    }

    newHistory->player1name = calloc(GAMESTATE_MAX_PLAYERNAME_LENGTH+1,sizeof(char));
    if(newHistory->player1name == NULL)
    {
        free(newHistory);
        return NULL;
    }
    int result = strcpy_s(newHistory->player1name,GAMESTATE_MAX_PLAYERNAME_LENGTH,player1name);
    if(result != 0)
    {
        free(newHistory->player1name);
        free(newHistory);
        return NULL;
    }

    newHistory->player2name = calloc(GAMESTATE_MAX_PLAYERNAME_LENGTH+1,sizeof(char));
    if(newHistory->player2name == NULL)
    {
        free(newHistory->player1name);
        free(newHistory);
        return NULL;
    }
    result = strcpy_s(newHistory->player2name,GAMESTATE_MAX_PLAYERNAME_LENGTH,player2name);
    if(result != 0)
    {
        free(newHistory->player1name);
        free(newHistory->player2name);
        free(newHistory);
        return NULL;
    }


    newHistory->boards = calloc(boardSize*boardSize + 1,sizeof(char*));
    if(newHistory->boards == NULL)
    {
        free(newHistory->player1name);
        free(newHistory->player2name);
        free(newHistory);
        return NULL;
    }



    newHistory->allocatedSize = boardSize*boardSize + 1;
    newHistory->boardSize = boardSize;
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
    free(targetGameHistory->player1name);
    free(targetGameHistory->player2name);

    free(targetGameHistory);

    return;
}

int appendBoardState(GAME_STATE_HISTORY *targetHistory, GAME_BOARD *board) {
    if(targetHistory == NULL || board == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    if(targetHistory->boardSize != board->boardSize)
    {
        return GAMEBOARD_BOARD_SIZE_ERROR;
    }

    if(targetHistory->steps == targetHistory->allocatedSize)
    {
        char** newBoards = realloc(targetHistory->boards,targetHistory->allocatedSize*2);
        if(newBoards == NULL)
        {
            return GAMEBOARD_NOMEM_ERROR;
        }
        targetHistory->boards = newBoards;
    }

    targetHistory->boards[targetHistory->steps] = malloc(board->boardSize * board->boardSize);
    int result = memcpy_s(targetHistory->boards[targetHistory->steps], board->boardSize * board->boardSize, board->board, board->boardSize * board->boardSize);
    if(result != 0)
    {
        free(targetHistory->boards[targetHistory->steps]);
        targetHistory->boards[targetHistory->steps] = 0;
        return GAMEBOARD_GENERAL_ERROR;
    }

    targetHistory->steps++;

    return GAMEBOARD_SUCCESS;
}

GAME_BOARD *getBoardFromState(GAME_STATE_HISTORY *targetHistory, int index) {
    if(targetHistory == NULL || index >= targetHistory->steps)
    {
        return NULL;
    }

    GAME_BOARD* newBoard = initNewBoard(targetHistory->boardSize);
    if(newBoard == NULL)
    {
        return NULL;
    }

    int result = memcpy_s(newBoard->board,newBoard->boardSize*newBoard->boardSize,
            targetHistory->boards[index],newBoard->boardSize*newBoard->boardSize);
    if(result != 0)
    {
        freeGameBoard(newBoard);
        return NULL;
    }

    return newBoard;
}

int updateBoardFromState(GAME_STATE_HISTORY *targetHistory, GAME_BOARD *targetBoard, int index) {
    if(targetHistory == NULL || targetBoard == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    if(index >= targetHistory->steps)
    {
        return GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR;
    }

    int result = memcpy_s(targetBoard->board, targetBoard->boardSize*targetBoard->boardSize,
    targetHistory->boards[index],targetBoard->boardSize*targetBoard->boardSize);
    if(result != 0)
    {
        return GAMEBOARD_GENERAL_ERROR;
    }

    return GAMEBOARD_SUCCESS;
}

int seekGameHistoryIterator(GAME_STATE_HISTORY *targetHistory, int seekIndex) {
    if(targetHistory == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    if(seekIndex >= targetHistory->steps)
    {
        return GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR;
    }

    targetHistory->iteratorIndex = seekIndex;

    return GAMEBOARD_SUCCESS;
}

int seekGameHistoryIteratorBeginning(GAME_STATE_HISTORY *targetHistory) {
    if(targetHistory == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    int result = seekGameHistoryIterator(targetHistory,0);
    if(result != GAMEBOARD_SUCCESS)
    {
        return GAMEBOARD_GENERAL_ERROR;
    }

    return GAMEBOARD_SUCCESS;
}

int seekGameHistoryIteratorCurrent(GAME_STATE_HISTORY *targetHistory) {
    if(targetHistory == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    int result = seekGameHistoryIterator(targetHistory,targetHistory->steps - 1);
    if(result != GAMEBOARD_SUCCESS)
    {
        return GAMEBOARD_GENERAL_ERROR;
    }

    return GAMEBOARD_SUCCESS;
}

GAME_BOARD *getNextBoardState(GAME_STATE_HISTORY *targetHistory) {
    if(targetHistory == NULL)
    {
        return NULL;
    }

    if(targetHistory->iteratorIndex == targetHistory->steps)
    {
        return NULL;
    }

    GAME_BOARD* newBoard = getBoardFromState(targetHistory,targetHistory->iteratorIndex);
    targetHistory->iteratorIndex++;

    return newBoard;
}

int updateBoardFromNextState(GAME_STATE_HISTORY *targetHistory, GAME_BOARD *targetBoard) {
    if(targetHistory == NULL || targetBoard == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    if(targetHistory->iteratorIndex == targetHistory->steps)
    {
        return GAMEBOARD_END_OF_ITERATION;
    }


    int result = updateBoardFromState(targetHistory,targetBoard,targetHistory->iteratorIndex);
    if(result != GAMEBOARD_SUCCESS)
    {
        return GAMEBOARD_GENERAL_ERROR;
    }
    targetHistory->iteratorIndex++;

    return GAMEBOARD_SUCCESS;
}

GAME_BOARD *getPreviousBoardState(GAME_STATE_HISTORY *targetHistory) {
    if(targetHistory == NULL)
    {
        return NULL;
    }

    if(targetHistory->iteratorIndex == -1)
    {
        return NULL;
    }

    GAME_BOARD* newBoard = getBoardFromState(targetHistory,targetHistory->iteratorIndex);
    targetHistory->iteratorIndex--;

    return newBoard;
}

int updateBoardFromPreviousBoardState(GAME_STATE_HISTORY *targetHistory, GAME_BOARD *targetBoard) {
    if(targetHistory == NULL || targetBoard == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    if(targetHistory->iteratorIndex == -1)
    {
        return GAMEBOARD_END_OF_ITERATION;
    }


    int result = updateBoardFromState(targetHistory,targetBoard,targetHistory->iteratorIndex);
    if(result != GAMEBOARD_SUCCESS)
    {
        return GAMEBOARD_GENERAL_ERROR;
    }
    targetHistory->iteratorIndex--;

    return GAMEBOARD_SUCCESS;
}
