# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        Click/Backends/SDLClickBackend.cpp
        Click/Click.cpp
        InputSystem.cpp)

set(HEADERS
        Click/Backends/SDLClickBackend.h
        Click/Click.h
        Click/ClickInputTypes.h
        InputSystem.h
        )

add_library(libs_input STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_input PRIVATE libs_utility sdl2_wrapper)
add_to_libs_folder(libs_input)

unset(SOURCES)
unset(HEADERS)