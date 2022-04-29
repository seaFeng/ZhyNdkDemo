# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_VERBOSE_MAKEFILE on)

message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR} ")
set(lib_src_Dir "${CMAKE_CURRENT_SOURCE_DIR}/tmp/${ANDROID_ABI}")

message(STATUS "ENV{HOME} = $ENV{HOME}/tmp/${ANDROID_ABI} ")
set(lib_build_DIR $ENV{HOME})

#file(MAKE_DIRECTORY )