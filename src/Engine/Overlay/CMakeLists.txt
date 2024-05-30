# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    RenderingOverlay.cpp
    ResourceOverlayHelper.cpp
    ResourcesOverlay.cpp)
    
set(HEADERS
    RenderingOverlay.h
    ResourceOverlayHelper.h
    ResourcesOverlay.h)

add_library(engine_overlay ${SOURCES} ${HEADERS})
target_link_libraries(engine_overlay
    PRIVATE
    engine_rendering
    engine_resource
    libs_overlay
    libs_utility
    glm)
add_to_engine_folder(engine_overlay)

unset(SOURCES)
unset(HEADERS)