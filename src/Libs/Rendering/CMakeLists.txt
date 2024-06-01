# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    DebugMessageHandler.cpp
    GeometryBuilder.cpp
    RenderQueue.cpp
    RenderSystem.cpp
    VertexAttributes.cpp
    RenderStep/RenderStep.cpp)
    
set(HEADERS    
    BaseBuffer.h
    DebugMessageHandler.h
    GeometryBuilder.h
    IndexBuffer.h
    RenderQueue.h
    RenderSystem.h
    VertexBuffer.h
    VertexAttributes.h
    RenderStep/RenderStep.h)

add_library(libs_rendering STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_rendering
    PUBLIC
    glad
    glm
    PRIVATE
    libs_utility
    libs_resource
    imgui
    sdl2_wrapper)
add_to_libs_folder(libs_rendering)

unset(SOURCES)
unset(HEADERS)