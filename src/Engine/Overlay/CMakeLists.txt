# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    CustomOverlayColors.cpp
    EngineCoreOverlay.cpp
    FsmOverlay.cpp
    RenderingOverlay.cpp
    ResourceOverlayHelper.cpp
    ResourcesOverlay.cpp)
    
set(HEADERS
    CustomOverlayColors.h
    EngineCoreOverlay.h
    FsmOverlay.h
    RenderingOverlay.h
    ResourceOverlayHelper.h
    ResourcesOverlay.h)

add_library(engine_overlay ${SOURCES} ${HEADERS})
target_link_libraries(engine_overlay
    PRIVATE
    libs_rendering
    libs_resource
    libs_ecs
    libs_overlay
    libs_utility
    libs_fsm
    glm)
add_to_engine_folder(engine_overlay)

unset(SOURCES)
unset(HEADERS)