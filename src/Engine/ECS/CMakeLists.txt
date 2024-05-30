# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    EcsSystem.cpp)
    
set(HEADERS
    EcsSystem.h
    Components/AlphaBlendedComponent.h
    Components/MeshComponent.h
    Components/TransformComponent.h)

add_library(engine_ecs ${SOURCES} ${HEADERS})
target_link_libraries(engine_ecs
    PUBLIC
    entt
    PRIVATE 
    engine_rendering
    engine_resource
    libs_utility)
add_to_engine_folder(engine_ecs)

unset(SOURCES)
unset(HEADERS)