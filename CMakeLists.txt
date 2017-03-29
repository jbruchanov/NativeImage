cmake_minimum_required(VERSION 3.6)
project(NativeImageApp)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)

set(SOURCE_FILES main.cpp)
add_executable(NativeImageApp ${SOURCE_FILES})