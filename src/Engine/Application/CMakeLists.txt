# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Application.cpp
    States/StartState.cpp
    States/MapState.cpp)
    
set(HEADERS
    Application.h
    States/StartState.h
    States/MapState.h)

add_library(engine_application ${SOURCES} ${HEADERS})
target_link_libraries(engine_application
    PRIVATE 
    engine_rendering
    libs_fsm
    libs_events
    sdl2_wrapper
    glm)
add_to_engine_folder(engine_application)

unset(SOURCES)
unset(HEADERS)