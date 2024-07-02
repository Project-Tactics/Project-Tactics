# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        InputSystem.cpp
        Backends/SDLInputBackend.cpp)

set(HEADERS
        InputSystem.h
        InputTypes.h
        Backends/SDLInputBackend.h)

add_library(libs_input STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_input PRIVATE libs_utility sdl2_wrapper)
add_to_libs_folder(libs_input)

unset(SOURCES)
unset(HEADERS)