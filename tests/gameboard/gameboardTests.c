#include "../../seatest/seatest.h"
#include "../../gameboard/gameboard.h"
//
// create a test...
//
void testBoardInit()
{
    GAME_BOARD* goodBoard = initNewBoard(10);
    GAME_BOARD* smallBoard = initNewBoard(3);
    GAME_BOARD* bigBoard = initNewBoard(100);
    GAME_BOARD* badBoardTooBig = initNewBoard(101);
    GAME_BOARD* badBoardTooSmall = initNewBoard(2);

    char emptySmallBoard[3] = {EmptyCell};
    for(int i = 0; i < 3; i++)
    {
        emptySmallBoard[i] = EmptyCell;
    }

    char emptyBigBoard[100] = {EmptyCell};
    for(int i = 0; i < 100; i++)
    {
        emptyBigBoard[i] = EmptyCell;
    }

    char emptyGoodBoard[10] = {EmptyCell};
    for(int i = 0; i < 10; i++)
    {
        emptyGoodBoard[i] = EmptyCell;
    }

    assert_int_equal(10,goodBoard->size);
    assert_int_equal(3,smallBoard->size);
    assert_int_equal(100,bigBoard->size);

    assert_n_array_equal(goodBoard->board,emptyGoodBoard,10);
    assert_n_array_equal(smallBoard->board,emptySmallBoard,3);
    assert_n_array_equal(bigBoard->board,emptyBigBoard,100);

    assert_int_equal(0,(int)badBoardTooBig);
    assert_int_equal(0,(int)badBoardTooSmall);

    freeGameBoard(goodBoard);
    freeGameBoard(smallBoard);
    freeGameBoard(bigBoard);
}

void testGameboardMakeMove()
{
    GAME_BOARD* testBoard = initNewBoard(10);
    int result = 0;
    result = makeMove(testBoard,0,0,CrossCell);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    assert_int_equal(CrossCell,testBoard->board[0]);

    result = makeMove(testBoard,testBoard->size - 1,testBoard->size - 1,CrossCell);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    assert_int_equal(CrossCell,testBoard->board[(testBoard->size)*(testBoard->size)-1]);

    result = makeMove(testBoard,0,0,ZeroCell);
    assert_int_equal(GAMEBOARD_INVALID_MOVE_CELL_OCCUPIED_ERROR,result);

    result = makeMove(testBoard,11,1,ZeroCell);
    assert_int_equal(GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR,result);

    result = makeMove(NULL,5,5,CrossCell);
    assert_int_equal(GAMEBOARD_NULL_PTR_ERROR,result);

    result = makeMove(testBoard,6,6,EmptyCell);
    assert_int_equal(GAMEBOARD_EMPTY_CELL_MOVE_ERROR,result);

    result = makeMove(testBoard,8,8,100);
    assert_int_equal(GAMEBOARD_UNKNOWN_CELL_TYPE_ERROR,result);

    freeGameBoard(testBoard);
}

void testGetCellTypeAt()
{
    GAME_BOARD* testBoard = initNewBoard(10);

    int result = 0;

    makeMove(testBoard,0,0,ZeroCell);
    result = getCellTypeAt(testBoard,0,0);
    assert_int_equal(ZeroCell,result);

    makeMove(testBoard,9,9,CrossCell);
    result = getCellTypeAt(testBoard,9,9);
    assert_int_equal(CrossCell,result);

    makeMove(testBoard,4,8,CrossCell);
    result = getCellTypeAt(testBoard,4,8);
    assert_int_equal(CrossCell,result);

    result = getCellTypeAt(testBoard,3,2);
    assert_int_equal(EmptyCell,result);

    result = getCellTypeAt(NULL,5,5);
    assert_int_equal(GAMEBOARD_NULL_PTR_ERROR,result);

    result = getCellTypeAt(testBoard,11,11);
    assert_int_equal(GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR,result);

    freeGameBoard(testBoard);
}

void winCondCheckRowOddAndEmpty()
{
    GAME_BOARD* testBoard = initNewBoard(5);

    int result = 0;

    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    makeMove(testBoard,1,0,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,1,2,CrossCell);
    makeMove(testBoard,1,3,CrossCell);
    makeMove(testBoard,1,4,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(CrossCell,result);

    freeGameBoard(testBoard);

    testBoard = initNewBoard(5);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    makeMove(testBoard,1,0,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,1,2,ZeroCell);
    makeMove(testBoard,1,3,CrossCell);
    makeMove(testBoard,1,4,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    result = checkBoardWinConditions(0);
    assert_int_equal(GAMEBOARD_NULL_PTR_ERROR,result);

    freeGameBoard(testBoard);
}

void winCondCheckRowEven()
{
    GAME_BOARD* testBoard = initNewBoard(4);

    int result = 0;

    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    makeMove(testBoard,1,0,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,1,2,CrossCell);
    makeMove(testBoard,1,3,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(CrossCell,result);

    freeGameBoard(testBoard);

    testBoard = initNewBoard(5);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    makeMove(testBoard,1,0,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,1,2,ZeroCell);
    makeMove(testBoard,1,3,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    result = checkBoardWinConditions(0);
    assert_int_equal(GAMEBOARD_NULL_PTR_ERROR,result);

    freeGameBoard(testBoard);
}

void winCondCheckColumnOdd()
{
    GAME_BOARD* testBoard = initNewBoard(5);

    int result = 0;

    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    makeMove(testBoard,0,1,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,2,1,CrossCell);
    makeMove(testBoard,3,1,CrossCell);
    makeMove(testBoard,4,1,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(CrossCell,result);

    freeGameBoard(testBoard);

    testBoard = initNewBoard(5);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    makeMove(testBoard,0,1,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,2,1,ZeroCell);
    makeMove(testBoard,3,1,CrossCell);
    makeMove(testBoard,4,1,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    result = checkBoardWinConditions(0);
    assert_int_equal(GAMEBOARD_NULL_PTR_ERROR,result);

    freeGameBoard(testBoard);
}

void winCondCheckColumnEven()
{
    GAME_BOARD* testBoard = initNewBoard(4);

    int result = 0;

    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    makeMove(testBoard,0,1,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,2,1,CrossCell);
    makeMove(testBoard,3,1,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(CrossCell,result);

    freeGameBoard(testBoard);

    testBoard = initNewBoard(4);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    makeMove(testBoard,0,1,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,2,1,ZeroCell);
    makeMove(testBoard,3,1,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    result = checkBoardWinConditions(0);
    assert_int_equal(GAMEBOARD_NULL_PTR_ERROR,result);

    freeGameBoard(testBoard);
}

void winCondCheckMainDiagonalOdd()
{
    GAME_BOARD* testBoard = initNewBoard(5);

    int result = 0;

    makeMove(testBoard,0,0,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,2,2,CrossCell);
    makeMove(testBoard,3,3,CrossCell);
    makeMove(testBoard,4,4,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(CrossCell,result);

    freeGameBoard(testBoard);

    testBoard = initNewBoard(5);
    makeMove(testBoard,0,0,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,2,2,ZeroCell);
    makeMove(testBoard,3,3,CrossCell);
    makeMove(testBoard,4,4,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    freeGameBoard(testBoard);
}

void winCondCheckMainDiagonalEven()
{
    GAME_BOARD* testBoard = initNewBoard(4);

    int result = 0;

    makeMove(testBoard,0,0,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,2,2,CrossCell);
    makeMove(testBoard,3,3,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(CrossCell,result);

    freeGameBoard(testBoard);

    testBoard = initNewBoard(4);
    makeMove(testBoard,0,0,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,2,2,ZeroCell);
    makeMove(testBoard,3,3,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    freeGameBoard(testBoard);
}

void winCondCheckAntidiagonalOdd()
{
    GAME_BOARD* testBoard = initNewBoard(5);

    int result = 0;

    makeMove(testBoard,4,0,CrossCell);
    makeMove(testBoard,3,1,CrossCell);
    makeMove(testBoard,2,2,CrossCell);
    makeMove(testBoard,1,3,CrossCell);
    makeMove(testBoard,0,4,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(CrossCell,result);

    freeGameBoard(testBoard);

    testBoard = initNewBoard(5);
    makeMove(testBoard,4,0,CrossCell);
    makeMove(testBoard,3,1,CrossCell);
    makeMove(testBoard,2,2,ZeroCell);
    makeMove(testBoard,1,3,CrossCell);
    makeMove(testBoard,0,4,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    freeGameBoard(testBoard);
}

void winCondCheckAntidiagonalEven()
{
    GAME_BOARD* testBoard = initNewBoard(4);

    int result = 0;

    makeMove(testBoard,3,0,CrossCell);
    makeMove(testBoard,2,1,CrossCell);
    makeMove(testBoard,1,2,CrossCell);
    makeMove(testBoard,0,3,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(CrossCell,result);

    freeGameBoard(testBoard);

    testBoard = initNewBoard(4);
    makeMove(testBoard,3,0,CrossCell);
    makeMove(testBoard,2,1,CrossCell);
    makeMove(testBoard,1,2,ZeroCell);
    makeMove(testBoard,0,3,CrossCell);
    result = checkBoardWinConditions(testBoard);
    assert_int_equal(GAMEBOARD_NO_WIN,result);

    freeGameBoard(testBoard);
}

void boardEncoderCheck()
{
    GAME_BOARD* testBoard = initNewBoard(3);
    makeMove(testBoard,0,0,CrossCell);
    makeMove(testBoard,0,2,ZeroCell);
    makeMove(testBoard,1,0,CrossCell);
    makeMove(testBoard,1,1,CrossCell);
    makeMove(testBoard,2,1,ZeroCell);
    char* buffer = encodeBoard(testBoard);
    char testBuffer[3*3+2] = {3,CrossCell,EmptyCell,ZeroCell,
                        CrossCell,CrossCell,EmptyCell,
                        EmptyCell,ZeroCell,EmptyCell,0};
    assert_n_array_equal(testBuffer,buffer,3*3+2);

    free(buffer);

    buffer = encodeBoard(NULL);
    assert_int_equal(NULL,buffer);

    freeGameBoard(testBoard);
}

void boardDecoderCheck()
{
    GAME_BOARD* originalBoard = initNewBoard(10);
    makeMove(originalBoard,3,1,CrossCell);
    makeMove(originalBoard,9,9,ZeroCell);
    makeMove(originalBoard,0,0,CrossCell);
    char* encodedBoard = encodeBoard(originalBoard);

    GAME_BOARD* decodedBoard = decodeBoard(encodedBoard);
    assert_int_equal(originalBoard->size,decodedBoard->size);
    assert_n_array_equal(originalBoard->board,decodedBoard->board,originalBoard->size);

    freeGameBoard(originalBoard);
    freeGameBoard(decodedBoard);
}

//
// put the test into a fixture...
//
void test_fixture_gameboard( void )
{
    test_fixture_start();
    run_test(testBoardInit);
    run_test(testGameboardMakeMove);
    run_test(testGetCellTypeAt);

    run_test(winCondCheckRowOddAndEmpty);
    run_test(winCondCheckRowEven);

    run_test(winCondCheckColumnOdd);
    run_test(winCondCheckColumnEven);

    run_test(winCondCheckMainDiagonalEven);
    run_test(winCondCheckMainDiagonalOdd);

    run_test(winCondCheckAntidiagonalEven);
    run_test(winCondCheckAntidiagonalOdd);

    run_test(boardEncoderCheck);
    run_test(boardDecoderCheck);
    test_fixture_end();
}

//
// put the fixture into a suite...
//
void all_tests( void )
{
    test_fixture_gameboard();
}

//
// run the suite!
//
int main( int argc, char** argv )
{
    return run_tests(all_tests);
}