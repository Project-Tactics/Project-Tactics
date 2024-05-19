# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    ResourceSystemInitializer.cpp
    Shader/ShaderLoader.cpp
    Shader/ShaderManager.cpp
    Texture/TextureLoader.cpp
    Texture/TextureManager.cpp)
    
set(HEADERS
    ResourceSystemInitializer.h
    Shader/ShaderLoader.h
    Shader/Shader.h
    Shader/ShaderManager.h
    Texture/Texture.h
    Texture/TextureLoader.h
    Texture/TextureManager.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(engine_resources ${SOURCES} ${HEADERS})
target_link_libraries(engine_resources
    PUBLIC
    libs_resource
    PRIVATE
    glad
    stb
    libs_utils)
set_property(TARGET engine_resources PROPERTY FOLDER "tactics/engine")
add_to_engine_folder(engine_resources)

unset(SOURCES)
unset(HEADERS)