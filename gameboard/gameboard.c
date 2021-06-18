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

    newBoard->boardSize = size;

    for(int i = 0; i < size*size; i++)
    {
        newBoard->board[i] = EmptyCell;
    }

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

    if(row < 0 || row >= targetBoard->boardSize || column < 0 || column >= targetBoard->boardSize)
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

    targetBoard->board[row*targetBoard->boardSize + column] = sign;

    return GAMEBOARD_SUCCESS;
}

CELL_TYPE getCellTypeAt(GAME_BOARD *targetBoard, char row, char column) {
    if(targetBoard == NULL || targetBoard->board == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    if(row < 0 || row >= targetBoard->boardSize || column < 0 || column >= targetBoard->boardSize)
    {
        return GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR;
    }

    return targetBoard->board[row*targetBoard->boardSize + column];
}

int checkBoardWinConditions(GAME_BOARD *targetBoard) {
    if(targetBoard == NULL || targetBoard->board == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    //Check rows
    for (char rowIndex = 0; rowIndex < targetBoard->boardSize; rowIndex++)
    {
        CELL_TYPE currentRowCellType = getCellTypeAt(targetBoard, rowIndex,0);
        if(currentRowCellType == EmptyCell)
        {
            continue; //Skip known non-winning rows
        }

        for (char columnIndex = 0; columnIndex < targetBoard->boardSize; columnIndex++)
        {
            if(getCellTypeAt(targetBoard,rowIndex,columnIndex) != currentRowCellType)
            {
                break;
            }

            //If we got to last cell in row without encountering other symbols, row is filled,
            if(columnIndex == targetBoard->boardSize - 1)
            {
                return currentRowCellType;
            }
        }
    }

    //Check columns
    for (char columnIndex = 0; columnIndex < targetBoard->boardSize; columnIndex++)
    {
        CELL_TYPE currentColumnCellType = getCellTypeAt(targetBoard,0,columnIndex);
        if(currentColumnCellType == EmptyCell)
        {
            continue; //Skip known non-winning columns
        }

        for (char rowIndex = 0; rowIndex < targetBoard->boardSize; rowIndex++)
        {
            if(getCellTypeAt(targetBoard,rowIndex,columnIndex) != currentColumnCellType)
            {
                break;
            }

            //If we got to last cell in column without encountering other symbols, column is filled,
            if(rowIndex == targetBoard->boardSize - 1)
            {
                return currentColumnCellType;
            }
        }
    }

    //If no wins found, check diagonals
    CELL_TYPE ULCellType = getCellTypeAt(targetBoard,0,0); //Upper left corner cell
    CELL_TYPE BLCellType = getCellTypeAt(targetBoard, targetBoard->boardSize - 1, 0);//Bottom left corner cell

    if(ULCellType != EmptyCell) {
        //Go through main diagonal
        for (char index = 0; index < targetBoard->boardSize; index++) {
            if (getCellTypeAt(targetBoard, index, index) != ULCellType) {
                break;
            }

            if (index == targetBoard->boardSize - 1) {
                return ULCellType;
            }
        }
    }

    if(BLCellType != EmptyCell) {
        //Go through antidiagonal
        for (char index = 0; index < targetBoard->boardSize; index++) {
            //Columns keep going right to left, but rows go from bottom to top, so they are boardSize-index-1
            if (getCellTypeAt(targetBoard, targetBoard->boardSize - 1 - index, index) != BLCellType) {
                break;
            }

            if (index == targetBoard->boardSize - 1) {
                return BLCellType;
            }
        }
    }

    return GAMEBOARD_NO_WIN;
}

char *encodeBoard(GAME_BOARD *targetBoard) {
    if(targetBoard == NULL || targetBoard->board == NULL)
    {
        return NULL;
    }

    int boardEncodingLength = targetBoard->boardSize * targetBoard->boardSize;

    char* sendBuffer = calloc(boardEncodingLength + 2,sizeof(char));
    if(sendBuffer == NULL)
    {
        return NULL;
    }
    sendBuffer[0] = targetBoard->boardSize;
    int result = memcpy_s(sendBuffer+1,boardEncodingLength,targetBoard->board,boardEncodingLength);
    if(result != 0)
    {
        free(sendBuffer);
        return 0;
    }

    return sendBuffer;
}

GAME_BOARD *decodeBoard(char *encoding) {
    if(encoding == NULL)
    {
        return NULL;
    }

    char boardSideLength = encoding[0];
    int encodingLength = strlen(encoding+1);
    if(encodingLength/boardSideLength != boardSideLength || boardSideLength < 3 || boardSideLength > 100)
    {
        return NULL;
    }

    GAME_BOARD* newBoard = initNewBoard(boardSideLength);
    if(newBoard == NULL)
    {
        return NULL;
    }

    int result = memcpy_s(newBoard->board,encodingLength,encoding+1,encodingLength);
    if(result != 0)
    {
        freeGameBoard(newBoard);
        return NULL;
    }

    return newBoard;
}

/*int drawCLIgameboard(GAME_BOARD *targetBoard) {
    if(targetBoard == NULL || targetBoard->board == NULL)
    {
        return GAMEBOARD_NULL_PTR_ERROR;
    }

    if(targetBoard->size > 10)
    {
        return GAMEBOARD_BOARD_TOO_LARGE;
    }

    for(int index = -1; index <= targetBoard->size; index++)
    {
        if(index == -1)
        {
            printf("%c",201);
            continue;
        }

        if(index == targetBoard->size)
        {
            printf("%c",187);
            continue;
        }

        printf("%c",205);
    }

    printf("\n");

    for(char rowIndex = 0; rowIndex < targetBoard->size; rowIndex++)
    {
        for(char columnIndex = -1; columnIndex <= targetBoard->size; columnIndex++)
        {
            if(columnIndex == -1 || columnIndex == targetBoard->size)
            {
                printf("%c",186);
                continue;
            }
            switch(getCellTypeAt(targetBoard,rowIndex,columnIndex))
            {
                case EmptyCell:
                    printf(" ");
                    break;

                case ZeroCell:
                    printf("O");
                    break;

                case CrossCell:
                    printf("X");
                    break;
            }
        }
        printf("\n");
    }

    for(int index = -1; index <= targetBoard->size; index++)
    {
        if(index == -1)
        {
            printf("%c",200);
            continue;
        }

        if(index == targetBoard->size)
        {
            printf("%c",188);
            continue;
        }

        printf("%c",205);
    }
}*/
