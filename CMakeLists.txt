cmake_minimum_required(VERSION 3.16)
project(TicTacToe C)

set(CMAKE_C_STANDARD 11)

find_package(Threads REQUIRED)

add_executable(TTTClient Client/client.c clientNetworking/clientNetworkWorker.c clientNetworking/clientNetworkWorker.h)
add_executable(TTTServer Server/server.c)

target_link_libraries(TTTClient ws2_32)
target_link_libraries(TTTClient Threads::Threads)
target_link_libraries(TTTServer ws2_32)
target_link_libraries(TTTServer Threads::Threads)

#Test executables
add_executable(gameboardTest seatest/seatest.c gameboard/gameboard.c tests/gameboard/gameboardTests.c)
add_executable(gamestateTest seatest/seatest.c gameboard/gameboard.c gamestate/gamestate.c gamestate/gamestate.h tests/gamestate/gamestateTests.c)
add_executable(userdataTest seatest/seatest.c userdata/userdata.c userdata/userdata.h tests/userdata/userdataTests.c)

add_executable(clientNetworkingTest seatest/seatest.c clientNetworking/clientNetworkWorker.c clientNetworking/packet.c clientNetworking/packet.h tests/clientNetworking/clientNetworkingTests.c clientNetworking/packetEnums.h)
target_link_libraries(clientNetworkingTest ws2_32)