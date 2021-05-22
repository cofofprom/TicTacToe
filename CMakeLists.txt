cmake_minimum_required(VERSION 3.16)
project(TicTacToe C)

set(CMAKE_C_STANDARD 11)

find_package(Threads REQUIRED)

add_executable(TTTClient Client/client.c )
add_executable(TTTServer Server/server.c )

target_link_libraries(TTTClient ws2_32)
target_link_libraries(TTTClient Threads::Threads)
target_link_libraries(TTTServer ws2_32)
target_link_libraries(TTTServer Threads::Threads)