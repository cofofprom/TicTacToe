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
    int currentStep;
    int iteratorIndex;
    char** boards;
} GAME_STATE_HISTORY;

///@brief Make new game state history
///@retval GAME_STATE_HISTORY* On success
///@retval NULL On error
///@param boardSize stored board size
GAME_STATE_HISTORY* initGameStateHistory(char boardSize);

///@brief Dispose of game state history
///@param targetGameHistory game state history to dispose of
void freeGameStateHistory(GAME_STATE_HISTORY* targetGameHistory);

///@brief Add new board state to game history
///@param targetHistory Game state history to append to
///@param board Board state to append
///@retval 0 On success
///@retval 1 NULL pointer encountered
///@retval 2 Memory alloc error (Out of memory probably)
int appendBoardState(GAME_STATE_HISTORY* targetHistory, GAME_BOARD* board);

///@brief Get board state at specified index
///@param targetHistory Game state history to pull board from
///@param index Index at witch to get a new board
///@retval GAME_BOARD* on success
///@retval NULL on Error
GAME_BOARD* getBoardFromState(GAME_STATE_HISTORY* targetHistory, int index);

///@brief Update existing game board from specified state
///@param targetHistory Game state history to update the board from
///@param targetBoard Game board to update
///@param index Index at which board state will be pulled
///@retval 0 On success
///@retval 1 NULL pointer encountered
///@retval 2 Index is out of range
int updateBoardFromState(GAME_STATE_HISTORY* targetHistory, GAME_BOARD* targetBoard, int index);

///@brief Set game state history iteration index to specified index
///@param targetHistory Game state history to modify
///@param seekIndex index to set game state history to
///@retval 0 On success
///@retval 1 NULL pointer encountered
///@retval 2 Seek index out of range
int seekGameHistoryIterator(GAME_STATE_HISTORY* targetHistory, int seekIndex);

///@brief Set game state history iteration index to game beginning
///@param targetHistory Game state history to modify
///@retval 0 On success
///@retval 1 NULL pointer encountered
int seekGameHistoryIteratorBeginning(GAME_STATE_HISTORY* targetHistory);

///@brief Set game state history iteration index to latest game state
///@param targetHistory Game state history to modify
///@retval 0 On success
///@retval 1 NULL pointer encountered
int seekGameHistoryIteratorCurrent(GAME_STATE_HISTORY* targetHistory);

///@brief Get next game board from game state history and increment the iterator
///@attention Game state history iterator is not reset upon reaching iteration end
///@details Get next board state, going forward in time
///@param targetHistory History to pull board state from
///@retval GAME_BOARD* On success
///@retval NULL On error or when reaching end of iteration
GAME_BOARD* getNextBoardState(GAME_STATE_HISTORY* targetHistory);

///@brief Update target board from game state history and incerement the iterator
///@attention Game state history iterator is not reset upon reaching end of iteration
///@details Update game board with next board state in history, going forward in time
///@param targetHistory Game state history to pull game board states from
///@param targetBoard Board to update
///@retval 0 On success
///@retval 1 NULL pointer encountered. If targetHistory was NULL, targetBoard state is not changed
///@retval 2 End of iteration
int updateBoardFromNextState(GAME_STATE_HISTORY* targetHistory, GAME_BOARD* targetBoard);

///@brief Get previous game board from history
///@attention Game state history iterator is not reset upon reaching iteration end
///@details Get previous board state, going back in time
///@param targetHistory History to pull from
///@retval GAME_BOARD* On success
///@retval NULL On error or reaching end of iteration
GAME_BOARD* getPreviousBoardState(GAME_STATE_HISTORY* targetHistory);

///@brief Update existing board from previous game state
///@attention Game state history iterator is not reset upon reaching end of iteration
///@details Update game board with previous game state, going back in time
///@param targetHistory Game state history to pull states from
///@param targetBoard Game board to update
///@retval 0 On success
///@retval 1 NULL pointer encountered. If targetHistory was NULL, targetBoard state is not changed
///@retval 2 End of iteration
int updateBoardFromPreviousBoardState(GAME_STATE_HISTORY* targetHistory, GAME_BOARD* targetBoard);

#endif //TICTACTOE_GAMESTATE_H
