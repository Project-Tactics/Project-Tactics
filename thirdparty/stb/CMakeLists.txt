# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(HEADERS
    stb/stb_image.h)

add_library(stb INTERFACE ${HEADERS})
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

unset(HEADERS)