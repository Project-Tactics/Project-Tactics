# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(HEADERS
    TransparentTypes.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(libs_utils INTERFACE ${HEADERS})
add_to_libs_folder(libs_utils)

unset(HEADERS)