cmake_minimum_required(VERSION 3.10)
project(GateServer)

set(CMAKE_CXX_STANDARD 14)

link_libraries(event_core)

add_executable(GateServer
        main.cpp)
