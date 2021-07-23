cmake_minimum_required(VERSION 3.13)
project(min2phase)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(min2phase
        src/CubeInfo.h
        src/CubieCube.h
        src/Coords.h
        src/Search.h
        src/CubeInfo.cpp
        src/CubieCube.cpp
        src/Coords.cpp
        src/Search.cpp
        src/Tools.cpp
        src/min2phase.cpp
        main.cpp)
