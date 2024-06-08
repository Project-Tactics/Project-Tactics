# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

file(GLOB_RECURSE HEADERS *.hpp *.h)

add_library(cereal INTERFACE ${HEADERS})
target_include_directories(cereal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
# disable some warnings
target_compile_options(cereal INTERFACE /wd5054)

unset(HEADERS)