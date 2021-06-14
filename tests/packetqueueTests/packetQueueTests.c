#include "../../seatest/seatest.h"
#include "../../clientNetworking/packetQueue.h"
//
// create a test...
//

void testPacketqueue()
{
    PACKET_QUEUE* testedQueue = initPacketQueue();
    char* packet1data = "12345";
    char* packet2data = "23456";
    char* packet3data = "34567";
    char* packet4data = "45678";
    char* packet5data = "56789";
    PACKET* packet1 = initPacketFromParams(ServicePacket,ServiceUserAction,BlacklistAction, packet1data);
    PACKET* packet2 = initPacketFromParams(DataRequestPacket,ServiceUserAction,BlacklistAction, packet2data);
    PACKET* packet3 = initPacketFromParams(DataSendPacket,ServiceUserAction,BlacklistAction, packet3data);
    PACKET* packet4 = initPacketFromParams(DataSendPacket,ServiceUserAction,BlacklistAction, packet4data);
    PACKET* packet5 = initPacketFromParams(ServicePacket,ServiceUserAction,BlacklistAction, packet5data);

    enqueuePacket(testedQueue,packet1);
    enqueuePacket(testedQueue,packet2);
    enqueuePacket(testedQueue,packet3);
    enqueuePacket(testedQueue,packet4);
    enqueuePacket(testedQueue,packet5);

    PACKET* packet1dequeued = dequeuePacket(testedQueue);
    PACKET* packet2dequeued = dequeuePacket(testedQueue);
    PACKET* packet3dequeued = dequeuePacket(testedQueue);
    PACKET* packet4dequeued = dequeuePacket(testedQueue);
    PACKET* packet5dequeued = dequeuePacket(testedQueue);

    assert_int_equal(packet1->packetType,packet1dequeued->packetType);
    assert_int_equal(packet2->packetType,packet2dequeued->packetType);
    assert_int_equal(packet3->packetType,packet3dequeued->packetType);
    assert_int_equal(packet4->packetType,packet4dequeued->packetType);
    assert_int_equal(packet5->packetType,packet5dequeued->packetType);

    assert_int_equal(packet1->packetSubtype,packet1dequeued->packetSubtype);
    assert_int_equal(packet2->packetSubtype,packet2dequeued->packetSubtype);
    assert_int_equal(packet3->packetSubtype,packet3dequeued->packetSubtype);
    assert_int_equal(packet4->packetSubtype,packet4dequeued->packetSubtype);
    assert_int_equal(packet5->packetSubtype,packet5dequeued->packetSubtype);

    assert_int_equal(packet1->packetCode,packet1dequeued->packetCode);
    assert_int_equal(packet2->packetCode,packet2dequeued->packetCode);
    assert_int_equal(packet3->packetCode,packet3dequeued->packetCode);
    assert_int_equal(packet4->packetCode,packet4dequeued->packetCode);
    assert_int_equal(packet5->packetCode,packet5dequeued->packetCode);

    assert_n_array_equal(packet1->packetData,packet1dequeued->packetData,5);
    assert_n_array_equal(packet2->packetData,packet2dequeued->packetData,5);
    assert_n_array_equal(packet3->packetData,packet3dequeued->packetData,5);
    assert_n_array_equal(packet4->packetData,packet4dequeued->packetData,5);
    assert_n_array_equal(packet5->packetData,packet5dequeued->packetData,5);

    freePacketQueue(testedQueue);

    freePacket(packet1);
    freePacket(packet2);
    freePacket(packet3);
    freePacket(packet4);
    freePacket(packet5);

    freePacket(packet1dequeued);
    freePacket(packet2dequeued);
    freePacket(packet3dequeued);
    freePacket(packet4dequeued);
    freePacket(packet5dequeued);
}

void test_fixture_packetqueue()
{
    test_fixture_start();
    run_test(testPacketqueue);
    test_fixture_end();
}

//
// put the fixture into a suite...
//
void all_tests( void )
{
    test_fixture_packetqueue();
}

//
// run the suite!
//
int main( int argc, char** argv )
{
    return run_tests(all_tests);
}