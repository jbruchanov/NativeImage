cmake_minimum_required(VERSION 3.6)
project(NativeImageApp)

add_definitions(-DDESKTOP -DDEBUG)

set(GOOGLE_TEST_SOURCE lib/googletest)
include_directories(
        ${GOOGLE_TEST_SOURCE}
        ${GOOGLE_TEST_SOURCE}/include)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(json11)

set(SOURCE_FILES main.cpp)
add_executable(NativeImageApp ${SOURCE_FILES})