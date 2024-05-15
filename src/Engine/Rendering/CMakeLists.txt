# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    RenderSystem.cpp
    ShaderLoader.cpp)
    
set(HEADERS
    RenderSystem.h
    ShaderLoader.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(engine_rendering ${SOURCES} ${HEADERS})
target_link_libraries(engine_rendering
    PRIVATE
    sdl2_wrapper
    glad)
set_property(TARGET engine_rendering PROPERTY FOLDER "tactics/engine")
add_to_engine_folder(engine_rendering)

unset(SOURCES)
unset(HEADERS)