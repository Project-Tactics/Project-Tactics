# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Application.cpp
    Engine.cpp)
    
set(HEADERS
    Application.h
    Engine.h)

add_library(engine_core ${SOURCES} ${HEADERS})
target_link_libraries(engine_core
    PUBLIC
    libs_fsm
    PRIVATE 
    engine_rendering
    engine_resource
    engine_ecs
    engine_overlay
    libs_event
    libs_overlay
    libs_utility
    libs_filesystem
    sdl2_wrapper
    glm)
add_to_engine_folder(engine_core)

unset(SOURCES)
unset(HEADERS)