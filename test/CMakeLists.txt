cmake_minimum_required(VERSION 3.6)
project(NativeImageTest)

enable_testing()

set(GOOGLE_TEST_SOURCE lib/googletest)
include_directories(
        ${GOOGLE_TEST_SOURCE}
        ${GOOGLE_TEST_SOURCE}/include)

set(SOURCE_FILES sample/_sample.cpp)

add_subdirectory(lib/googletest)

add_executable(NativeImageTest ${SOURCE_FILES})

target_link_libraries(NativeImageTest NativeImageLib gtest gtest_main)
