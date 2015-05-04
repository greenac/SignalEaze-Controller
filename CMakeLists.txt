cmake_minimum_required(VERSION 2.8.4)
project(signalEaze)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    signalEaze.ino)

add_executable(signalEaze ${SOURCE_FILES})