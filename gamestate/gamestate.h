#ifndef TICTACTOE_GAMESTATE_H
#define TICTACTOE_GAMESTATE_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "../gameboard/gameboard.h"

#define GAMESTATE_MAX_PLAYERNAME_LENGTH 32

typedef struct gamestate_history_struct
{
    short steps;
    char boardSize;
    char* player1name;
    char* player2name;
    char winner; //-1 - current, 0 - tie, 1 - player1 won, 2 - player2 won
    int allocatedSize;
    int iteratorIndex;
    char** boards;
} GAME_STATE_HISTORY;

///@brief Make new game state history
///@retval GAME_STATE_HISTORY* On success
///@retval NULL On error
///@param boardSize stored board boardSize
GAME_STATE_HISTORY* initGameStateHistory(char boardSize, char* player1name, char* player2name);

///@brief Dispose of game state history
///@param targetGameHistory game state history to dispose of
void freeGameStateHistory(GAME_STATE_HISTORY* targetGameHistory);

///@brief Add new board state to game history
///@param targetHistory Game state history to append to
///@param board Board state to append
///@retval GAMEBOARD_SUCCESS On success
///@retval GAMEBOARD_NULL_PTR_ERROR NULL pointer encountered
///@retval GAMEBOARD_NOMEM_ERROR Memory alloc error (Out of memory probably)
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
///@retval GAMEBOARD_SUCCESS On success
///@retval GAMEBOARD_NULL_PTR_ERROR NULL pointer encountered
///@retval GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR Index is out of range
int updateBoardFromState(GAME_STATE_HISTORY* targetHistory, GAME_BOARD* targetBoard, int index);

///@brief Set game state history iteration index to specified index
///@param targetHistory Game state history to modify
///@param seekIndex index to set game state history to
///@retval 0 On success
///@retval GAMEBOARD_NULL_PTR_ERROR NULL pointer encountered
///@retval GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR Seek index out of range
int seekGameHistoryIterator(GAME_STATE_HISTORY* targetHistory, int seekIndex);

///@brief Set game state history iteration index to game beginning
///@param targetHistory Game state history to modify
///@retval GAMEBOARD_SUCCESS On success
///@retval GAMEBOARD_NULL_PTR_ERROR NULL pointer encountered
int seekGameHistoryIteratorBeginning(GAME_STATE_HISTORY* targetHistory);

///@brief Set game state history iteration index to latest game state
///@param targetHistory Game state history to modify
///@retval 0 On success
///@retval GAMEBOARD_NULL_PTR_ERROR NULL pointer encountered
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
///@retval GAMEBOARD_SUCCESS On success
///@retval GAMEBOARD_NULL_PTR_ERROR NULL pointer encountered. If targetHistory was NULL, targetBoard state is not changed
///@retval GAMEBOARD_END_OF_ITERATION End of iteration
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
///@retval GAMEBOARD_SUCCESS On success
///@retval GAMEBOARD_NULL_PTR_ERROR NULL pointer encountered. If targetHistory was NULL, targetBoard state is not changed
///@retval GAMEBOARD_END_OF_ITERATION End of iteration
int updateBoardFromPreviousBoardState(GAME_STATE_HISTORY* targetHistory, GAME_BOARD* targetBoard);


///@brief Encodes game state history for network transmission
///@param targetHistory Game state history to encode
///@retval NULL On error
///@retval char* Encoded game state history
char* encodeGameHistory(GAME_STATE_HISTORY* targetHistory);

///@brief Decodes game state history from encoded string
///@param encoding Encoded game state history
///@retval NULL On error
///@retval GAME_STATE_HISTORY* Decoded game state history
GAME_STATE_HISTORY* decodeGameHistory(char* encoding);

#endif //TICTACTOE_GAMESTATE_H
