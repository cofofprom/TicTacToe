#include "gameboard.h"

GAME_BOARD *initNewBoard(char size)
{
    if(size > MAX_BOARD_SIZE || size < MIN_BOARD_SIZE)
    {
        return NULL;
    }

    GAME_BOARD* newBoard = calloc(1,sizeof(GAME_BOARD));
    if(newBoard == NULL)
    {
        return NULL;
    }

    newBoard->board = calloc(size*size,sizeof(char));
    if(newBoard->board == NULL)
    {
        free(newBoard);
        return NULL;
    }

    newBoard->size = size;

    return newBoard;
}

void freeGameBoard(GAME_BOARD *gameBoard) {
    if(gameBoard == NULL)
    {
        return;
    }

    if(gameBoard->board != NULL)
    {
        free(gameBoard->board);
    }

    free(gameBoard);

    return;
}

int makeMove(GAME_BOARD *targetBoard, char row, char column, CELL_TYPE sign) {
    if(targetBoard == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    if(row < 0 || row >= targetBoard->size || column < 0 || column >= targetBoard->size)
    {
        return GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR;
    }

    if(sign == EmptyCell)
    {
        return GAMEBOARD_EMPTY_CELL_MOVE_ERROR;
    }

    if(sign != CrossCell && sign != ZeroCell)
    {
        return GAMEBOARD_UNKNOWN_CELL_TYPE_ERROR;
    }

    if(getCellTypeAt(targetBoard,row,column) != EmptyCell)
    {
        return  GAMEBOARD_INVALID_MOVE_CELL_OCCUPIED_ERROR;
    }

    targetBoard->board[row*targetBoard->size + column] = sign;

    return GAMEBOARD_SUCCESS;
}

CELL_TYPE getCellTypeAt(GAME_BOARD *targetBoard, char row, char column) {
    if(targetBoard == NULL || targetBoard->board == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    if(row < 0 || row >= targetBoard->size || column < 0 || column >= targetBoard->size)
    {
        return GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR;
    }

    return targetBoard->board[row*targetBoard->size + column];
}

int checkBoardWinConditions(GAME_BOARD *targetBoard) {
    if(targetBoard == NULL || targetBoard->board == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    //Check rows
    for (char rowIndex = 0; rowIndex < targetBoard->size; rowIndex++)
    {
        CELL_TYPE currentRowCellType = getCellTypeAt(targetBoard, rowIndex,0);
        if(currentRowCellType == EmptyCell)
        {
            continue; //Skip known non-winning rows
        }

        for (char columnIndex = 0; columnIndex < targetBoard->size; columnIndex++)
        {
            if(getCellTypeAt(targetBoard,rowIndex,columnIndex) != currentRowCellType)
            {
                break;
            }

            //If we got to last cell in row without encountering other symbols, row is filled,
            if(columnIndex == targetBoard->size - 1)
            {
                return currentRowCellType;
            }
        }
    }

    //Check columns
    for (char columnIndex = 0; columnIndex < targetBoard->size; columnIndex++)
    {
        CELL_TYPE currentColumnCellType = getCellTypeAt(targetBoard,0,columnIndex);
        if(currentColumnCellType == EmptyCell)
        {
            continue; //Skip known non-winning columns
        }

        for (char rowIndex = 0; rowIndex < targetBoard->size; rowIndex++)
        {
            if(getCellTypeAt(targetBoard,rowIndex,columnIndex) != currentColumnCellType)
            {
                break;
            }

            //If we got to last cell in column without encountering other symbols, column is filled,
            if(rowIndex == targetBoard->size - 1)
            {
                return currentColumnCellType;
            }
        }
    }

    //If no wins found, check diagonals
    CELL_TYPE ULCellType = getCellTypeAt(targetBoard,0,0); //Upper left corner cell
    CELL_TYPE BLCellType = getCellTypeAt(targetBoard,targetBoard->size-1,0);//Bottom left corner cell

    if(ULCellType != EmptyCell) {
        //Go through main diagonal
        for (char index = 0; index < targetBoard->size; index++) {
            if (getCellTypeAt(targetBoard, index, index) != ULCellType) {
                break;
            }

            if (index == targetBoard->size - 1) {
                return ULCellType;
            }
        }
    }

    if(BLCellType != EmptyCell) {
        //Go through antidiagonal
        for (char index = 0; index < targetBoard->size; index++) {
            //Columns keep going right to left, but rows go from bottom to top, so they are size-index-1
            if (getCellTypeAt(targetBoard, targetBoard->size - 1 - index, index) != BLCellType) {
                break;
            }

            if (index == targetBoard->size - 1) {
                return BLCellType;
            }
        }
    }

    return GAMEBOARD_NO_WIN;
}
