#ifndef TICTACTOE_GAMEBOARD_H
#define TICTACTOE_GAMEBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

#define MAX_BOARD_SIZE 100
#define MIN_BOARD_SIZE 3

#define GAMEBOARD_SUCCESS 0
#define GAMEBOARD_NULL_PTR_ERROR -1
#define GAMEBOARD_INVALID_MOVE_CELL_OCCUPIED_ERROR -2
#define GAMEBOARD_EMPTY_CELL_MOVE_ERROR -3
#define GAMEBOARD_UNKNOWN_CELL_TYPE_ERROR -4
#define GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR -5
#define GAMEBOARD_NO_WIN -6

typedef enum cell_types_enum{EmptyCell = 1,ZeroCell = 2,CrossCell = 3} CELL_TYPE;

///@brief Struct to contain game board info
typedef struct game_board_struct
{
    char size;
    char* board;
}GAME_BOARD;

///@brief Initialize new board
///@details Game board initialization function
///@param size Board side length. Cannot be smaller than 3 or larger than 100
///@retval NULL On error
///@retval GAME_BOARD* On success
GAME_BOARD* initNewBoard(char size);

///@brief Dispose of game board
///@details A safe game board disposal function
///@param gameBoard Game board to dispose of
void freeGameBoard(GAME_BOARD* gameBoard);

///@brief Make a move on the board
///@details Make a move on the board, performing move validity check
///@param targetBoard Targeted board to make a move on
///@param row Zero-based row index at witch a move is made
///@param column Zero-based column index at which a move is made
///@param sign What sign is placed on a targeted cell
///@retval 0 Success
///@retval GAMEBOARD_NULL_PTR_ERROR NULL pointer
///@retval GAMEBOARD_INVALID_MOVE_CELL_OCCUPIED_ERROR Invalid move
///@retval GAMEBOARD_EMPTY_CELL_MOVE_ERROR Attempt to place an empty cell
///@retval GAMEBOARD_UNKNOWN_CELL_TYPE_ERROR Unknown sign type
int makeMove(GAME_BOARD* targetBoard, char row, char column, CELL_TYPE sign);

///@brief Gets cell type at indexes specified
///@param targetBoard Pointer to target board
///@param row Row at which to get cell type
///@param column Column at which to get cell type
///@retval GAMEBOARD_NUL_PTR_ERROR NULL pointer
///@retval GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR Index out of range
///@retval CELL_TYPE Cell type at specified position
CELL_TYPE getCellTypeAt(GAME_BOARD* targetBoard, char row, char column);

///@brief Check board for win positions, if found, return sign that won
///@details Performs win condition check and returns winning side on success
///@param targetBoard Board to perform a check on
///@retval GAMEBOARD_NULL_PTR_ERROR NULL pointer
///@retval GAMEBOARD_NO_WIN No winning side
///@retval CrossCell Crosses won
///@retval ZeroCell Zeros won
int checkBoardWinConditions(GAME_BOARD* targetBoard);

///@brief Encode board for network transmission
///@details Encodes board for network transmission, as a null-terminated string
///@param targetBoard board to encode
///@retval NULL On error
///@retval Encoded string
char* encodeBoard(GAME_BOARD* targetBoard);

///@brief Decode board from encoded string
///@details Decode game board from string
///@param encoding Gameboard encoding string
///@retval NULL On error
///@retval GAME_BOARD* On success
GAME_BOARD* decodeBoard(char* encoding);

#endif //TICTACTOE_GAMEBOARD_H
