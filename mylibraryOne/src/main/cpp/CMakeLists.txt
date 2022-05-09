# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_VERBOSE_MAKEFILE on)

message(STATUS "11111 CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR} ")
set(lib_src_Dir "${CMAKE_CURRENT_SOURCE_DIR}")

message(STATUS "22222 ENV{HOME} = $ENV{HOME}/tmp/${ANDROID_ABI} ")
message(STATUS "3333 CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "4444 PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
set(lib_build_DIR "D:/code/lenovo/worker/ZhyNdkDemo/mylibraryOne/buildout")

file(MAKE_DIRECTORY ${lib_build_DIR})
file(MAKE_DIRECTORY ${lib_build_DIR}/gmath)
file(MAKE_DIRECTORY ${lib_build_DIR}/gperf)

message(STATUS "55555 lib_src_DIR/gmath = ${lib_src_Dir}/gmath")
add_subdirectory(${lib_src_Dir}/gmath ${lib_build_DIR}/gmath)
add_subdirectory(${lib_src_Dir}/gperf ${lib_build_DIR}/gperf)