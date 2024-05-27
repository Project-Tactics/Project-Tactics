# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    ResourceSystemInitializer.cpp
    Mesh/Mesh.cpp
    Mesh/MeshLoader.cpp
    Shader/Shader.cpp
    Shader/ShaderLoader.cpp
    Texture/Texture.cpp
    Texture/TextureLoader.cpp
    Material/Material.cpp
    Material/MaterialLoader.cpp
    Material/Uniforms.cpp)
    
set(HEADERS
    ResourceSystemInitializer.h
    Mesh/Mesh.h
    Mesh/MeshLoader.h
    Shader/Shader.h
    Shader/ShaderLoader.h
    Texture/Texture.h
    Texture/TextureLoader.h
    Material/Material.h
    Material/MaterialLoader.h
    Material/Uniforms.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(engine_resource ${SOURCES} ${HEADERS})
target_link_libraries(engine_resource
    PUBLIC
    libs_resource
    PRIVATE
    glad
    stb
    libs_utility
    assimp_wrapper)
set_property(TARGET engine_resource PROPERTY FOLDER "tactics/engine")
add_to_engine_folder(engine_resource)

unset(SOURCES)
unset(HEADERS)