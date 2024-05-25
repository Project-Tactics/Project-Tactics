# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Application.cpp
    States/StartState.cpp
    States/DemoState.cpp
    Overlay/DebugOverlay.cpp)
    
set(HEADERS
    Application.h
    Overlay/DebugOverlay.h
    States/StartState.h
    States/DemoState.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(engine_application ${SOURCES} ${HEADERS})
target_link_libraries(engine_application
    PRIVATE 
    engine_rendering
    engine_resources
    engine_ecs
    libs_fsm
    libs_events
    libs_overlay
    libs_utils
    sdl2_wrapper
    glm)
add_to_engine_folder(engine_application)

unset(SOURCES)
unset(HEADERS)