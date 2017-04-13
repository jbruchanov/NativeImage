cmake_minimum_required(VERSION 3.6)
project(NativeImageLib)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCE_FILES *.[h|c]*)

find_package(JNI)

if (JNI_FOUND)
    include_directories(${JNI_INCLUDE_DIRS})
else ()
    message(STATUS "")
    message(STATUS "Including files from JAVA_HOME:'$ENV{JAVA_HOME}'")
    message(STATUS "This is mostlikely because of Windows, be sure you have updated jni_md.h if cygwin!'")
    message(STATUS "Check ${CMAKE_CURRENT_SOURCE_DIR}/java/jni_md.h")
    message(STATUS "")
    include_directories($ENV{JAVA_HOME}/include)
    include_directories($ENV{JAVA_HOME}/include/win32)
endif ()

add_library(NativeImageLib STATIC ${SOURCE_FILES})
target_link_libraries(NativeImageLib jpeg json11 png)