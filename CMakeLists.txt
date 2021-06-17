cmake_minimum_required(VERSION 3.16)
project(TicTacToe C)

set(CMAKE_C_STANDARD 11)

find_package(Threads REQUIRED)

set(SOURCES  clientNetworking/clientNetworkWorker.h
        clientNetworking/clientNetworkWorker.c
        clientNetworking/packet.h
        clientNetworking/packet.c
        clientNetworking/packetEnums.h
        clientNetworking/packetQueue.h
        clientNetworking/packetQueue.c
        SetField/SetField.c
        SetField/SetField.h
        Client/clientGraphics.c
        Client/clientGraphics.h
        Client/eventDispatcher.c
        Client/eventDispatcher.h
        Client/serverUserdata.c
        Client/serverUserdata.h
        gameboard/gameboard.c
        gameboard/gameboard.h
        gamestate/gamestate.c
        gamestate/gamestate.h
        Server/utils.c
        Server/utils.h)

add_executable(TTTClient Client/client.c ${SOURCES} )

add_executable(TTTServer Server/server.c ${SOURCES})

add_executable(Server2 ${SOURCES} Client/Server2.c Client/serverUserdata.c Client/serverUserdata.h Client/serverNetworkWorker.c)
target_link_libraries(Server2 ws2_32)
target_link_libraries(Server2 Threads::Threads)

target_link_libraries(TTTClient ws2_32)
target_link_libraries(TTTClient Threads::Threads)
target_link_libraries(TTTServer ws2_32)
target_link_libraries(TTTServer Threads::Threads)

#Test executables
add_executable(gameboardTest seatest/seatest.c gameboard/gameboard.c tests/gameboard/gameboardTests.c)
add_executable(gamestateTest seatest/seatest.c gameboard/gameboard.c gamestate/gamestate.c gamestate/gamestate.h tests/gamestate/gamestateTests.c)
add_executable(userdataTest seatest/seatest.c userdata/userdata.c userdata/userdata.h tests/userdata/userdataTests.c)
add_executable(playerlistTest seatest/seatest.c tests/playerlistTests/playerlistTests.c playerlist/playerlist.c playerlist/playerlist.h userdata/userdata.c)
add_executable(packetQueueTest seatest/seatest.c clientNetworking/packet.c clientNetworking/packetEnums.h clientNetworking/packetQueue.c clientNetworking/packetQueue.h tests/packetqueueTests/packetQueueTests.c)
add_executable(SetFieldTest  seatest/seatest.c seatest/seatest.h tests/SetField/SetFieldTest.c SetField/SetField.c SetField/SetField.h)
add_executable(CNWtestServer tests/clientNetworking/CNWtestServer.c clientNetworking/clientNetworkWorker.c clientNetworking/packet.h clientNetworking/packet.c clientNetworking/packetQueue.c clientNetworking/packetEnums.h)
target_link_libraries(CNWtestServer ws2_32)
target_link_libraries(CNWtestServer Threads::Threads)

add_executable(CNWtestClient tests/clientNetworking/CNWtestClient.c clientNetworking/clientNetworkWorker.c clientNetworking/packet.h clientNetworking/packet.c clientNetworking/packetQueue.c clientNetworking/packetEnums.h)
target_link_libraries(CNWtestClient ws2_32)
target_link_libraries(CNWtestClient Threads::Threads)

add_executable(clientNetworkingTest seatest/seatest.c clientNetworking/clientNetworkWorker.c clientNetworking/packet.c clientNetworking/packet.h tests/clientNetworking/clientNetworkingTests.c clientNetworking/packetEnums.h clientNetworking/packetQueue.c clientNetworking/packetQueue.h)
target_link_libraries(clientNetworkingTest ws2_32)
target_link_libraries(clientNetworkingTest Threads::Threads)
