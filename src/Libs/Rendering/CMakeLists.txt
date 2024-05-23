# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    VertexDefinition.cpp)
    
set(HEADERS    
    BaseBuffer.h
    IndexBuffer.h
    VertexBuffer.h
    VertexDefinition.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(libs_rendering STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_rendering
    PUBLIC
    glad
    glm
    PRIVATE
    libs_utils)
add_to_libs_folder(libs_rendering)

unset(SOURCES)
unset(HEADERS)