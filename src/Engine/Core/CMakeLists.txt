# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Application.cpp
    Engine.cpp
    Overlay/DebugOverlay.cpp)
    
set(HEADERS
    Application.h
    Engine.h
    Overlay/DebugOverlay.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(engine_core ${SOURCES} ${HEADERS})
target_link_libraries(engine_core
    PUBLIC
    libs_fsm
    PRIVATE 
    engine_rendering
    engine_resource
    engine_ecs
    libs_event
    libs_overlay
    libs_utility
    sdl2_wrapper
    glm)
add_to_engine_folder(engine_core)

unset(SOURCES)
unset(HEADERS)