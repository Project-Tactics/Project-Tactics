# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Application.cpp)
    
set(HEADERS
    Application.h)

add_library(engine_application ${SOURCES} ${HEADERS})
target_link_libraries(engine_application
    PRIVATE 
    engine_rendering
    sdl2_wrapper)

unset(SOURCES)
unset(HEADERS)