#include "../../seatest/seatest.h"
#include "../../playerlist/playerlist.h"
//
// create a test...
//
void testPlayerlist()
{
    PLAYERLIST* testPlayerlist = initPlayerlist(5,Top100List);
    USER_DATA* testPlayerdata = initNamedUserData("PLAYER1");

    appendToPlayerlist(testPlayerlist,testPlayerdata);

    testPlayerdata->gameCount = 10;
    testPlayerdata->lossCount = 15;
    testPlayerdata->winCount = 4;

    appendToPlayerlist(testPlayerlist,testPlayerdata);

    testPlayerdata->username[6] = '2';
    testPlayerdata->winCount = 100;
    testPlayerdata->lossCount = 12345;
    testPlayerdata->gameCount = 1199;

    appendToPlayerlist(testPlayerlist,testPlayerdata);

    assert_int_equal(3,testPlayerlist->playerListLength);
    assert_int_equal(5,testPlayerlist->allocatedLength);
    assert_int_equal(0,testPlayerlist->playerdataList[0]->gameCount);
    assert_int_equal(0,testPlayerlist->playerdataList[0]->lossCount);
    assert_int_equal(0,testPlayerlist->playerdataList[0]->winCount);
    assert_n_array_equal("PLAYER1",testPlayerlist->playerdataList[0]->username,7);

    assert_int_equal(10,testPlayerlist->playerdataList[1]->gameCount);
    assert_int_equal(15,testPlayerlist->playerdataList[1]->lossCount);
    assert_int_equal(4,testPlayerlist->playerdataList[1]->winCount);
    assert_n_array_equal("PLAYER1",testPlayerlist->playerdataList[1]->username,7);

    assert_int_equal(1199,testPlayerlist->playerdataList[2]->gameCount);
    assert_int_equal(12345,testPlayerlist->playerdataList[2]->lossCount);
    assert_int_equal(100,testPlayerlist->playerdataList[2]->winCount);
    assert_n_array_equal("PLAYER2",testPlayerlist->playerdataList[2]->username,7);

    char* Top100Encoding = encodeTop100Playerlist(testPlayerlist);
    char* friendblacklistEncoding = encodeFriendAndBlacklist(testPlayerlist);

    PLAYERLIST* decodedTop100List = decodeTop100Playerlist(Top100Encoding);
    PLAYERLIST* decodedFriendList = decodeFriendAndBlacklist(friendblacklistEncoding);

    assert_int_equal(3,decodedTop100List->allocatedLength);
    assert_int_equal(3,decodedTop100List->playerListLength);

    assert_int_equal(0,decodedTop100List->playerdataList[0]->gameCount);
    assert_int_equal(0,decodedTop100List->playerdataList[0]->lossCount);
    assert_int_equal(0,decodedTop100List->playerdataList[0]->winCount);
    assert_n_array_equal("PLAYER1",decodedTop100List->playerdataList[0]->username,7);

    assert_int_equal(0,decodedTop100List->playerdataList[1]->gameCount);
    assert_int_equal(15,decodedTop100List->playerdataList[1]->lossCount);
    assert_int_equal(4,decodedTop100List->playerdataList[1]->winCount);
    assert_n_array_equal("PLAYER1",decodedTop100List->playerdataList[1]->username,7);

    assert_int_equal(0,decodedTop100List->playerdataList[2]->gameCount);
    assert_int_equal(12345,decodedTop100List->playerdataList[2]->lossCount);
    assert_int_equal(100,decodedTop100List->playerdataList[2]->winCount);
    assert_n_array_equal("PLAYER2",decodedTop100List->playerdataList[2]->username,7);

    assert_int_equal(3,decodedFriendList->allocatedLength);
    assert_int_equal(3,decodedFriendList->playerListLength);

    assert_int_equal(0,decodedFriendList->playerdataList[0]->gameCount);
    assert_int_equal(0,decodedFriendList->playerdataList[0]->lossCount);
    assert_int_equal(0,decodedFriendList->playerdataList[0]->winCount);
    assert_n_array_equal("PLAYER1",decodedFriendList->playerdataList[0]->username,7);

    assert_int_equal(0,decodedFriendList->playerdataList[1]->gameCount);
    assert_int_equal(0,decodedFriendList->playerdataList[1]->lossCount);
    assert_int_equal(0,decodedFriendList->playerdataList[1]->winCount);
    assert_n_array_equal("PLAYER1",decodedFriendList->playerdataList[1]->username,7);

    assert_int_equal(0,decodedFriendList->playerdataList[2]->gameCount);
    assert_int_equal(0,decodedFriendList->playerdataList[2]->lossCount);
    assert_int_equal(0,decodedFriendList->playerdataList[2]->winCount);
    assert_n_array_equal("PLAYER2",decodedFriendList->playerdataList[2]->username,7);
}


//
// put the test into a fixture...
//
void test_fixture_playerlist( void )
{
    test_fixture_start();
    run_test(testPlayerlist);
    test_fixture_end();
}

//
// put the fixture into a suite...
//
void all_tests( void )
{
    test_fixture_playerlist();
}

//
// run the suite!
//
int main( int argc, char** argv )
{
    return run_tests(all_tests);
}