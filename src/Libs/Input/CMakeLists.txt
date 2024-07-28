# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        Click/Backend/ClickSDLBackend.cpp
        Click/Click.cpp
        Click/ClickModifiers.cpp
        Click/ClickConditions.cpp
        InputSerialization.cpp
        InputSystem.cpp)

set(HEADERS
        Click/Backend/ClickSDLBackend.h
        Click/Click.h
        Click/ClickEnumTypes.h
        Click/ClickEvents.h
        Click/ClickInputTypes.h
        Click/ClickModifiers.h
        Click/ClickConditions.h
        InputSerialization.h
        InputSystem.h)

add_library(libs_input STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_input PRIVATE libs_event libs_utility sdl2_wrapper)
add_to_libs_folder(libs_input)

unset(SOURCES)
unset(HEADERS)