# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Resource.cpp
    ResourcePathHelper.cpp
    ResourceSystem.cpp
    ResourceManager.cpp
    ResourcePack/ResourcePackManager.cpp)
    
set(HEADERS
    Resource.h
    ResourcePathHelper.h
    ResourceSystem.h
    ResourceManager.h
    ResourcePack/ResourcePackManager.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(libs_resource STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_resource
    PUBLIC 
    libs_scripting
    PRIVATE
    libs_utils)
add_to_libs_folder(libs_resource)

unset(SOURCES)
unset(HEADERS)