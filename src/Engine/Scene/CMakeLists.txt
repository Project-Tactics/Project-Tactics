# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Scene.cpp
    SceneSystem.cpp)
    
set(HEADERS
    Scene.h
    SceneSystem.h)

add_library(engine_scene ${SOURCES} ${HEADERS})
target_link_libraries(engine_scene
    PUBLIC
    libs_ecs
    PRIVATE
    libs_resource
    libs_rendering
    )
set_property(TARGET engine_scene PROPERTY FOLDER "tactics/engine")
add_to_engine_folder(engine_scene)

unset(SOURCES)
unset(HEADERS)