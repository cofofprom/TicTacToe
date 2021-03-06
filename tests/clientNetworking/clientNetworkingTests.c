#include "../../seatest/seatest.h"
#include "../../clientNetworking/clientNetworkWorker.h"
#include "../../clientNetworking/packet.h"
//
// create a test...
//
void testPacketEncoding()
{
    PACKET* newPacket = initPacket();
    char packetData[] = {1,2,3,4,5};
    newPacket->packetData = calloc( 5 + 1,sizeof(char));
    memcpy(newPacket->packetData,packetData,5);

    newPacket->packetType = ServicePacket;
    newPacket->packetSubtype = ServiceUserAction;
    newPacket->packetCode = GameRequestAction;
    newPacket->packetLength = 3 + 5; //Codes + data, since subcode is required

    char* encoding = encodePacket(newPacket);

    PACKET* decodedPacket = decodePacket(encoding);

    assert_int_equal(newPacket->packetCode,decodedPacket->packetCode);
    assert_int_equal(newPacket->packetLength, decodedPacket->packetLength);
    assert_int_equal(newPacket->packetSubtype,decodedPacket->packetSubtype);
    assert_int_equal(newPacket->packetType,decodedPacket->packetType);
    assert_n_array_equal(newPacket->packetData,decodedPacket->packetData,5);

    printPacketDebug(newPacket);
    printPacketDebug(decodedPacket);

    freePacket(newPacket);
    freePacket(decodedPacket);
    free(encoding);
}

void testNodataEncodingDecoding()
{
    PACKET* testPacket = initPacketFromParams(DataRequestPacket, LoginRequest, 0, 0);
    char* encoding = encodePacket(testPacket);
    PACKET* decodedPacket = decodePacket(encoding);

    assert_int_equal(testPacket->packetCode,decodedPacket->packetCode);
    assert_int_equal(testPacket->packetType,decodedPacket->packetType);
    assert_int_equal(testPacket->packetSubtype,decodedPacket->packetSubtype);
    assert_int_equal(testPacket->packetLength,decodedPacket->packetLength);
    assert_int_equal(testPacket->packetData,decodedPacket->packetData);

    printPacketDebug(testPacket);
    printPacketDebug(decodedPacket);

    freePacket(testPacket);
    freePacket(decodedPacket);

    free(encoding);
}

//
// put the test into a fixture...
//
void test_fixture_packet( void )
{
    test_fixture_start();
    run_test(testPacketEncoding);
    run_test(testNodataEncodingDecoding);
    test_fixture_end();
}

//
// put the fixture into a suite...
//
void all_tests( void )
{
    test_fixture_packet();
}

//
// run the suite!
//
int main( int argc, char** argv )
{
    return run_tests(all_tests);
}