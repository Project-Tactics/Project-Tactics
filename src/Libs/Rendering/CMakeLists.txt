# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    DebugMessageHandler.cpp
    GeometryBuilder.cpp
    Particle/Particles.cpp
    Particle/ParticlesModifier.cpp
    Particle/ParticleSystem.cpp
    RenderCalls/RenderCalls.cpp
    RenderQueue.cpp
    RenderSystem.cpp
    RenderStep/RenderStep.cpp
    VertexAttributes.cpp)
    
set(HEADERS    
    BaseBuffer.h
    DebugMessageHandler.h
    GeometryBuilder.h
    IndexBuffer.h
    Particle/Particles.h
    Particle/ParticlesModifier.h
    Particle/ParticlesSerialization.h
    Particle/ParticlesTypes.h
    Particle/ParticleSystem.h
    RenderCalls/RenderCalls.h
    RenderQueue.h
    RenderSystem.h
    RenderStep/RenderStep.h
    VertexBuffer.h
    VertexAttributes.h
    Viewport.h)

set(LIB_DEPEDENCIES
    PUBLIC
    glad
    glm
    PRIVATE
    libs_utility
    libs_resource
    libs_event
    imgui
    sdl2_wrapper)

add_library(libs_rendering STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_rendering ${LIB_DEPEDENCIES})
add_to_libs_folder(libs_rendering)

add_library(libs_rendering_headless STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_rendering_headless ${LIB_DEPEDENCIES})
target_compile_definitions(libs_rendering_headless PRIVATE TACTICS_HEADLESS)
add_to_libs_folder(libs_rendering_headless)

unset(SOURCES)
unset(HEADERS)