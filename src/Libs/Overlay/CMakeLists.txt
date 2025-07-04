# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        OverlaySystem.cpp
        OverlayStyleHelper.cpp
        ExampleOverlay.cpp
        MainOverlay.cpp)

set(HEADERS
        Overlay.h
        OverlaySystem.h
        OverlayStyleHelper.h
        OverlayTypes.h
        ExampleOverlay.h
        MainOverlay.h)

add_library(libs_overlay STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_overlay PUBLIC imgui imgui_node_editor libs_resource PRIVATE libs_utility inicpp)
add_to_libs_folder(libs_overlay)

unset(SOURCES)
unset(HEADERS)