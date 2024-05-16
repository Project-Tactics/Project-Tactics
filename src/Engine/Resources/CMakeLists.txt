# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    ResourceSystem.cpp)
    
set(HEADERS
    ResourceSystem.h
    Resource.h
    ShaderResource.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(engine_resources ${SOURCES} ${HEADERS})
target_link_libraries(engine_resources
    PRIVATE
    libs_utils
    lua_wrapper)
set_property(TARGET engine_resources PROPERTY FOLDER "tactics/engine")
add_to_engine_folder(engine_resources)

unset(SOURCES)
unset(HEADERS)