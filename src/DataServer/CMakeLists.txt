cmake_minimum_required(VERSION 3.10)
project(DBServer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

link_libraries(event_core pq hiredis pthread)

add_executable(DBServer
        ../util/EventChannel.cpp
        ../util/Buffer.cpp
        ../util/WorkThread.cpp
        ../util/FdWatcher.cpp
        ../util/ThreadManager.cpp
        ../network/EventLoop.cpp
        ../network/TCPServer.cpp
        ../network/TCPClient.cpp
        ../network/TCPAcceptor.cpp
        ../network/TCPConnection.cpp
        ../network/TCPConnector.cpp
        ../log/Log.cpp

        ../DataServer/PGManager.cpp
        ../DataServer/RedisManager.cpp

        main.cpp)
