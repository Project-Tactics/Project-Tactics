# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Camera.cpp
    DebugMessageHandler.cpp
    RenderSteps/ClearColorRenderStep.cpp
    RenderSteps/DrawSomethingRenderStep.cpp
    RenderSteps/ImGuiRenderSteps.cpp
    RenderSteps/RenderStep.cpp
    RenderQueue.cpp
    RenderSystem.cpp)
    
set(HEADERS
    Camera.h
    DebugMessageHandler.h
    RenderSteps/ClearColorRenderStep.h
    RenderSteps/DrawSomethingRenderStep.h
    RenderSteps/ImGuiRenderSteps.h
    RenderSteps/RenderStep.h
    RenderQueue.h
    RenderSystem.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(engine_rendering ${SOURCES} ${HEADERS})
target_link_libraries(engine_rendering
    PUBLIC
    glad
    glm
    PRIVATE
    engine_resources
    libs_overlay
    libs_utils
    imgui
    sdl2_wrapper)
add_to_engine_folder(engine_rendering)

unset(SOURCES)
unset(HEADERS)