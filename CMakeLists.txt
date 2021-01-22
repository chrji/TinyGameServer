cmake_minimum_required(VERSION 3.10)
project(TinyGameServer)

set(CMAKE_CXX_STANDARD 14)

link_libraries(pq)

link_libraries(event_core)


add_executable(TinyGameServer src/main.cpp)