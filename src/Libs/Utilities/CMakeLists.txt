# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(HEADERS
    Exception.h
    Math.h
    TransparentTypes.h
    UUID.h)

set(SOURCES
    Exception.cpp
    Math.cpp
    UUID.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(libs_utils STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_utils PUBLIC glm fmt::fmt)
add_to_libs_folder(libs_utils)

unset(HEADERS)