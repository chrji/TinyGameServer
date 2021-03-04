cmake_minimum_required(VERSION 3.10)
project(TinyGameServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
endif ()

file(GLOB NETWORK_MODULE src/network/*.cpp)
file(GLOB UTIL_MODULE src/util/*.cpp)
file(GLOB DATA_MODULE src/data/*.cpp)

include_directories(./src)
link_libraries(event_core pq hiredis pthread)

message("Compile GateServer")
add_executable(GateServer
        ${NETWORK_MODULE}
        ${UTIL_MODULE}
        server/GateServer.cpp)

message("Compile DataServer")
add_executable(DataServer
        ${NETWORK_MODULE}
        ${UTIL_MODULE}
        ${DATA_MODULE}
        server/DataServer.cpp)


