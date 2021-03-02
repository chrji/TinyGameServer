cmake_minimum_required(VERSION 3.10)
project(TinyGameServer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

link_libraries(pq)
link_libraries(event_core)
link_libraries(pthread)

add_executable(TinyGameServer
        src/util/EventChannel.cpp
        src/util/Buffer.cpp
        src/util/WorkThread.cpp
        src/util/FdWatcher.cpp
        src/util/ThreadManager.cpp
        src/network/Network.cpp
        src/network/EventLoop.cpp
        src/network/TCPServer.cpp
        src/network/TCPClient.cpp
        src/network/TCPAcceptor.cpp
        src/network/TCPConnection.cpp
        src/network/TCPConnector.cpp
        src/log/Log.cpp

        src/main.cpp)
