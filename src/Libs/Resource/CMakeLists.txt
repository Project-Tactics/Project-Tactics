# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    IniFile/IniFile.cpp
    IniFile/IniFileLoader.cpp
    Resource.cpp
    ResourcePathHelper.cpp
    ResourceSystem.cpp
    ResourceManager.cpp
    ResourcePack/ResourcePackManager.cpp)
    
set(HEADERS
    IniFile/IniFile.h
    IniFile/IniFileLoader.h
    Resource.h
    ResourceLoader.h
    ResourcePathHelper.h
    ResourceSystem.h
    ResourceManager.h
    ResourcePack/ResourcePackManager.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(libs_resource STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_resource
    PUBLIC
    inicpp
    json_wrapper
    libs_utility)
add_to_libs_folder(libs_resource)

unset(SOURCES)
unset(HEADERS)