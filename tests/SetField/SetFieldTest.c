#include "../../seatest/seatest.h"
#include "../../SetField/SetField.h"
//
// create a test...
//
void testHandleFunctions()
{
    HWND StdHandle = makeWindow();
    assert_true(StdHandle != INVALID_HANDLE_VALUE);

    HWND ConsoleHandle = GetConsoleWindow();
    assert_true(ConsoleHandle != NULL);

//    clearWindow(StdHandle);
//    resetConsole(StdHandle);

    int xSize = findWidthOfWindow(), ySize = findHeightOfWindow();
    assert_int_equal(120, xSize);
    assert_int_equal(30, ySize);
}

//
// put the test into a fixture...
//
void test_fixture_handle_functions( void )
{
    test_fixture_start();
    run_test(testHandleFunctions);
    test_fixture_end();
}

//
// put the fixture into a suite...
//
void all_tests( void )
{
    test_fixture_handle_functions();
}

//
// run the suite!
//
int main( int argc, char** argv )
{
    run_tests(all_tests);
    _getch();
    return 0;
}