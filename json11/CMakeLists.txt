cmake_minimum_required(VERSION 3.6)
project(json11)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCE_FILES *.cpp)

add_library(json11 STATIC ${SOURCE_FILES})
