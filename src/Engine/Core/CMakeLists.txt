# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Application.cpp
    Engine.cpp
    ResourceSystemInitializer.cpp
    RenderSteps/PrepareViewportRenderStep.cpp
    RenderSteps/DrawMeshesRenderStep.cpp
    RenderSteps/ImGuiRenderSteps.cpp)
    
set(HEADERS
    Application.h
    Engine.h
    ResourceSystemInitializer.h
    RenderSteps/PrepareViewportRenderStep.h
    RenderSteps/DrawMeshesRenderStep.h
    RenderSteps/ImGuiRenderSteps.h)

add_library(engine_core ${SOURCES} ${HEADERS})
target_link_libraries(engine_core
    PUBLIC
    libs_fsm
    libs_resource
    PRIVATE 
    engine_overlay
    libs_rendering
    libs_event
    libs_overlay
    libs_ecs
    libs_utility
    libs_filesystem)
add_to_engine_folder(engine_core)

unset(SOURCES)
unset(HEADERS)