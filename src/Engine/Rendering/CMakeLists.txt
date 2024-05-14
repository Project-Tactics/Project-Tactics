# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    RenderSystem.cpp)
    
set(HEADERS
    RenderSystem.h)

add_library(engine_rendering ${SOURCES} ${HEADERS})
target_link_libraries(engine_rendering
    PRIVATE
    sdl2_wrapper
    glad)

unset(SOURCES)
unset(HEADERS)