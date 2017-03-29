cmake_minimum_required(VERSION 3.6)
project(NativeImageLib)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCE_FILES *.cpp)

add_library(NativeImageLib STATIC ${SOURCE_FILES})