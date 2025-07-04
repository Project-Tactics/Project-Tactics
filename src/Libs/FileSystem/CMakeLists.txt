cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        FileHandle/IniFileHandle.cpp
        FileHandle/JsonFileHandle.cpp
        FileHandle/StringFileHandle.cpp
        FileLoader.cpp
        FileSystem.cpp
        PathHelper.cpp)

set(HEADERS
        FileHandle/FileHandle.h
        FileHandle/IniFileHandle.h
        FileHandle/JsonFileHandle.h
        FileHandle/StringFileHandle.h
        FileLoader.h
        FileSystem.h
        PathHelper.h)

add_library(libs_filesystem STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_filesystem PUBLIC libs_utility inicpp)
add_to_libs_folder(libs_filesystem)

unset(SOURCES)
unset(HEADERS)
