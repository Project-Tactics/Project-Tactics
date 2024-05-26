# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    ResourceSystemInitializer.cpp
    Mesh/Mesh.cpp
    Mesh/MeshManager.cpp
    Shader/Shader.cpp
    Shader/ShaderManager.cpp
    Texture/Texture.cpp
    Texture/TextureManager.cpp
    Material/Material.cpp
    Material/MaterialManager.cpp)
    
set(HEADERS
    ResourceSystemInitializer.h
    Mesh/Mesh.h
    Mesh/MeshManager.h
    Shader/Shader.h
    Shader/ShaderManager.h
    Texture/Texture.h
    Texture/TextureManager.h
    Material/Material.h
    Material/MaterialManager.h)

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