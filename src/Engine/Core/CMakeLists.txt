# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Application.cpp
    DefaultFsmExternalController.cpp
    Engine.cpp
    ResourceSystemInitializer.cpp
    RenderSteps/PrepareCameraViewportRenderStep.cpp
    RenderSteps/DrawDebugRenderStep.cpp
    RenderSteps/DrawMeshesRenderStep.cpp
    RenderSteps/DrawUiRenderStep.cpp
    RenderSteps/ImGuiRenderSteps.cpp)
    
set(HEADERS
    Application.h
    DefaultFsmExternalController.h
    Engine.h
    ResourceSystemInitializer.h
    RenderSteps/PrepareCameraViewportRenderStep.h
    RenderSteps/DrawDebugRenderStep.h
    RenderSteps/DrawMeshesRenderStep.h
    RenderSteps/DrawUiRenderStep.h
    RenderSteps/ImGuiRenderSteps.h)

add_library(engine_core ${SOURCES} ${HEADERS})
target_link_libraries(engine_core
    PUBLIC
    libs_fsm
    libs_resource
    sdl2_wrapper
    PRIVATE 
    engine_overlay
    libs_rendering
    libs_event
    libs_overlay
    libs_ecs
    libs_utility
    libs_filesystem
    libs_input
    libs_ui
    libs_physics)
add_to_engine_folder(engine_core)

unset(SOURCES)
unset(HEADERS)