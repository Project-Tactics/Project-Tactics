# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        OverlaySystem.cpp
        ExampleOverlay.cpp
        ScriptedOverlay.cpp)

set(HEADERS
        Overlay.h
        OverlaySystem.h
        ExampleOverlay.h
        ScriptedOverlay.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(libs_overlay STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_overlay PRIVATE imgui lua_wrapper sol2 libs_utils)
add_to_libs_folder(libs_overlay)

unset(SOURCES)
unset(HEADERS)