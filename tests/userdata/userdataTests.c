#include "../../seatest/seatest.h"
#include "../../userdata/userdata.h"
//
// create a test...
//
void testUserdataEncoding()
{
    USER_DATA* newUserData = initNamedUserData("PLAYERNAME");

    newUserData->gameCount = 100;
    newUserData->lossCount = 73;
    newUserData->winCount = 100-73;
    newUserData->userStatus = OnlineIngame;

    char* encoding = encodeUserData(newUserData);

    USER_DATA* decodedUserData = decodeUserData(encoding);

    assert_int_equal(newUserData->gameCount,decodedUserData->gameCount);
    assert_int_equal(newUserData->lossCount,decodedUserData->lossCount);
    assert_int_equal(newUserData->winCount,decodedUserData->winCount);
    assert_int_equal(newUserData->userStatus,decodedUserData->userStatus);
    assert_n_array_equal(newUserData->username,decodedUserData->username,strlen("PLAYERNAME"));

    freeUserData(newUserData);
    freeUserData(decodedUserData);
    free(encoding);
}

//
// put the test into a fixture...
//
void test_fixture_userdata( void )
{
    test_fixture_start();
    run_test(testUserdataEncoding);
    test_fixture_end();
}

//
// put the fixture into a suite...
//
void all_tests( void )
{
    test_fixture_userdata();
}

//
// run the suite!
//
int main( int argc, char** argv )
{
    return run_tests(all_tests);
}