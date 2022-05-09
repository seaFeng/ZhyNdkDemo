# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_VERBOSE_MAKEFILE on)

add_library(gmath STATIC src/gmath.c)

set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../distribution)

set_target_properties(gmath PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${distribution_DIR}/gmath/lib/${ANDROID_ABI}")

add_custom_command(TARGET gmath POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
        copy "${CMAKE_CURRENT_SOURCE_DIR}/src/gmath.h" "${distribution_DIR}/gmath/include/gmath.h"
        COMMENT "Copying gmath to output directory")




