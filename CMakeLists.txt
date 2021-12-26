cmake_minimum_required(VERSION 3.13)
project(min2phase)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(min2phase
        src/info.h
        src/CubieCube.h
        src/coords.h
        src/Search.h
        src/tests.h
        src/info.cpp
        src/CubieCube.cpp
        src/coords.cpp
        src/Search.cpp
        src/tools.cpp
        src/min2phase.cpp
        src/tests.cpp
        main.cpp )

add_executable(benchmark
        src/info.h
        src/CubieCube.h
        src/coords.h
        src/Search.h
        src/tests.h
        src/info.cpp
        src/CubieCube.cpp
        src/coords.cpp
        src/Search.cpp
        src/tools.cpp
        src/min2phase.cpp
        src/tests.cpp
        benchmark.cpp )


add_executable(tests
        src/info.h
        src/CubieCube.h
        src/coords.h
        src/Search.h
        src/tests.h
        src/info.cpp
        src/CubieCube.cpp
        src/coords.cpp
        src/Search.cpp
        src/tools.cpp
        src/min2phase.cpp
        src/tests.cpp
        tests.cpp)