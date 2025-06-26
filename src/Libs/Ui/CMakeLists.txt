# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        UiSystem.cpp
        jab/JabTypes.cpp
        )

set(HEADERS
        UiSystem.h
        jab/JabTypes.h
        )

add_library(libs_ui STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_ui PRIVATE libs_utility)
add_to_libs_folder(libs_ui)

unset(SOURCES)
unset(HEADERS)