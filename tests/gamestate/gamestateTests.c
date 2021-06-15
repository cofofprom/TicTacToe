#include "../../gamestate/gamestate.h"
#include "../../seatest/seatest.h"
//
// create a test...
//

void testHistoryInit()
{
    GAME_STATE_HISTORY* testHistory = initGameStateHistory(3,"PLAYERONE","PLAYERTWO");
    assert_true(testHistory != NULL);
    assert_true(testHistory->boardSize = 5);
    assert_true(testHistory->boards != NULL);
    assert_true(testHistory->steps == 0);
    assert_true(testHistory->allocatedSize == 3*3 + 1);
    assert_string_equal("PLAYERONE",testHistory->player1name);
    assert_string_equal("PLAYERTWO",testHistory->player2name);
    freeGameStateHistory(testHistory);

    testHistory = initGameStateHistory(5, "PLONE","PLTWO");
    assert_true(testHistory != NULL);
    assert_true(testHistory->boardSize = 5);
    assert_true(testHistory->boards != NULL);
    assert_true(testHistory->steps == 0);
    assert_true(testHistory->allocatedSize == 5*5 + 1);
    assert_string_equal("PLONE",testHistory->player1name);
    assert_string_equal("PLTWO",testHistory->player2name);
    freeGameStateHistory(testHistory);

    testHistory = initGameStateHistory(101,"test","test2");
    assert_true(testHistory == NULL);

    testHistory = initGameStateHistory(2,"test","test2");
    assert_true(testHistory == NULL);
}

void testHistoryAppendRenameUpdate()
{
    GAME_STATE_HISTORY* testHistory = initGameStateHistory(3,"PLAYERONE","PLAYERTWO");
    GAME_BOARD* testBoard = initNewBoard(3);
    makeMove(testBoard,1,1,ZeroCell);
    int result = appendBoardState(testHistory,testBoard);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    assert_n_array_equal(testBoard->board,testHistory->boards[0],testBoard->boardSize*testBoard->boardSize);

    GAME_BOARD* newBoard = getBoardFromState(testHistory,0);
    assert_n_array_equal(testBoard->board,newBoard->board,testBoard->boardSize*testBoard->boardSize);

    makeMove(testBoard,2,2,CrossCell);
    result = appendBoardState(testHistory,testBoard);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    assert_n_array_equal(testBoard->board,testHistory->boards[1],testBoard->boardSize*testBoard->boardSize);

    result = updateBoardFromState(testHistory,newBoard,1);
    assert_n_array_equal(testBoard->board,newBoard->board,testBoard->boardSize*testBoard->boardSize);

    result = updateBoardFromState(testHistory,testBoard,3);
    assert_int_equal(GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR,result);

    freeGameStateHistory(testHistory);
    freeGameBoard(testBoard);
    freeGameBoard(newBoard);
}

void test_fixture_gamehistory_base( void )
{
    test_fixture_start();
    run_test(testHistoryInit);
    run_test(testHistoryAppendRenameUpdate);
    test_fixture_end();
}

void testHistoryIteratorSeek()
{
    GAME_STATE_HISTORY* testHistory = initGameStateHistory(3,"PLAYERONE","PLAYERTWO");
    GAME_BOARD* testBoard = initNewBoard(3);
    appendBoardState(testHistory, testBoard);
    appendBoardState(testHistory, testBoard);

    int result = seekGameHistoryIterator(testHistory,0);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    assert_int_equal(0,testHistory->iteratorIndex);

    result = seekGameHistoryIterator(testHistory,1);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    assert_int_equal(1,testHistory->iteratorIndex);

    result = seekGameHistoryIterator(testHistory,2);
    assert_int_equal(GAMEBOARD_INDEX_OUT_OF_RANGE_ERROR,result);
    assert_int_equal(1,testHistory->iteratorIndex);

    result = seekGameHistoryIteratorBeginning(testHistory);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    assert_int_equal(0,testHistory->iteratorIndex);

    result = seekGameHistoryIteratorCurrent(testHistory);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    assert_int_equal(1,testHistory->iteratorIndex);

    appendBoardState(testHistory,testBoard);

    result = seekGameHistoryIteratorCurrent(testHistory);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    assert_int_equal(2,testHistory->iteratorIndex);

    freeGameStateHistory(testHistory);
    freeGameBoard(testBoard);
}

void testHistoryIterator()
{
    GAME_STATE_HISTORY* testHistory = initGameStateHistory(3,"PLAYERONE","PLAYERTWO");
    GAME_BOARD* testBoard = initNewBoard(3);
    int boardLength = 3*3;

    appendBoardState(testHistory,testBoard);

    makeMove(testBoard,1,1,CrossCell);
    appendBoardState(testHistory,testBoard);

    makeMove(testBoard,2,2,ZeroCell);
    appendBoardState(testHistory,testBoard);

    seekGameHistoryIteratorBeginning(testHistory);

    GAME_BOARD* secondBoard = initNewBoard(3);
    freeGameBoard(testBoard);

    testBoard = getNextBoardState(testHistory);
    assert_n_array_equal(testBoard->board,secondBoard->board,boardLength);

    int result = updateBoardFromNextState(testHistory,testBoard);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    makeMove(secondBoard,1,1,CrossCell);
    assert_n_array_equal(testBoard->board,secondBoard->board,boardLength);

    result = updateBoardFromNextState(testHistory,testBoard);
    assert_int_equal(GAMEBOARD_SUCCESS,result);
    makeMove(secondBoard,2,2,ZeroCell);
    assert_n_array_equal(testBoard->board,secondBoard->board,boardLength);

    result = updateBoardFromNextState(testHistory,testBoard);
    assert_int_equal(GAMEBOARD_END_OF_ITERATION,result);

    freeGameBoard(testBoard);
    freeGameBoard(secondBoard);
    freeGameStateHistory(testHistory);
}

void test_fixture_gamehistory_iterator()
{
    test_fixture_start();
    run_test(testHistoryIteratorSeek);
    run_test(testHistoryIterator);
    test_fixture_end();
}

void testHistoryEncoding()
{
    GAME_STATE_HISTORY* testHistory = initGameStateHistory(3,"PLAYERONE","PLAYERTWO");
    GAME_BOARD* testBoard = initNewBoard(3);

    appendBoardState(testHistory,testBoard);

    makeMove(testBoard,1,1,CrossCell);
    appendBoardState(testHistory,testBoard);

    makeMove(testBoard,2,2,ZeroCell);
    appendBoardState(testHistory,testBoard);

    makeMove(testBoard,0,1,CrossCell);
    appendBoardState(testHistory,testBoard);

    makeMove(testBoard,0,0,ZeroCell);
    appendBoardState(testHistory,testBoard);

    char* outputBuffer = encodeGameHistory(testHistory);

    GAME_STATE_HISTORY* decodedHistory = decodeGameHistory(outputBuffer);

    assert_int_equal(testHistory->winner,decodedHistory->winner);
    assert_int_equal(testHistory->steps,testHistory->steps);
    assert_string_equal(testHistory->player1name,testHistory->player1name);
    assert_string_equal(testHistory->player2name,decodedHistory->player2name);
    for(int i = 0; i < decodedHistory->steps; i++)
    {
        assert_n_array_equal(testHistory->boards[i],decodedHistory->boards[i],3*3);
    }
    freeGameStateHistory(testHistory);
    freeGameStateHistory(decodedHistory);
    free(outputBuffer);
    freeGameBoard(testBoard);
}

void test_fixture_gamehistory_encoding()
{
    test_fixture_start();
    run_test(testHistoryEncoding);
    test_fixture_end();
}

//
// put the fixture into a suite...
//
void all_tests( void )
{
    test_fixture_gamehistory_base();
    test_fixture_gamehistory_iterator();
    test_fixture_gamehistory_encoding();
}

//
// run the suite!
//
int main( int argc, char** argv )
{
    return run_tests(all_tests);
}