cmake_minimum_required(VERSION 3.6)
project(NativeImageLib)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCE_FILES *.cpp)

find_package(JNI)

if (JNI_FOUND)
    include_directories(${JNI_INCLUDE_DIRS})
else()
    message(ERROR missing JNI)
endif()


add_library(NativeImageLib STATIC ${SOURCE_FILES} JNI_JpegImage.h)target_link_libraries(NativeImageLib jpeg json11)
