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

    char emptySmallBoard[3] = {0};
    char emptyBigBoard[100] = {0};
    char emptyGoodBoard[10] = {0};

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
    assert_int_equal(result,GAMEBOARD_SUCCESS);
    assert_int_equal(1,testBoard->board[0]);

    result = makeMove(testBoard,testBoard->size - 1,testBoard->size - 1,CrossCell);
    assert_int_equal(result,GAMEBOARD_SUCCESS);
    assert_int_equal(1,testBoard->board[(testBoard->size)*(testBoard->size)-1]);

    result = makeMove(testBoard,0,0,ZeroCell);
    assert_int_equal(result, GAMEBOARD_INVALID_MOVE_CELL_OCCUPIED_ERROR);

    result = makeMove(testBoard,11,1,ZeroCell);
    assert_int_equal(result,GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR);

    result = makeMove(NULL,5,5,CrossCell);
    assert_int_equal(result,GAMEBOARD_NULL_PTR_ERROR);

    result = makeMove(testBoard,6,6,EmptyCell);
    assert_int_equal(result,GAMEBOARD_EMPTY_CELL_MOVE_ERROR);

    result = makeMove(testBoard,8,8,3);
    assert_int_equal(result,GAMEBOARD_UNKNOWN_CELL_TYPE_ERROR);

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