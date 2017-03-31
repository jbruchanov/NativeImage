cmake_minimum_required(VERSION 3.6)
project(NativeImageLib)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCE_FILES *.[h|c]*)

find_package(JNI)

if (JNI_FOUND)
    include_directories(${JNI_INCLUDE_DIRS})
else()
    message(ERROR missing JNI)
endif()

add_library(NativeImageLib STATIC ${SOURCE_FILES})
target_link_libraries(NativeImageLib jpeg json11)