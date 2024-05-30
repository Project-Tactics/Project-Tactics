# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    GeometryBuilder.cpp
    VertexAttributes.cpp)
    
set(HEADERS    
    BaseBuffer.h
    GeometryBuilder.h
    IndexBuffer.h
    VertexBuffer.h
    VertexAttributes.h)

add_library(libs_rendering STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_rendering
    PUBLIC
    glad
    glm
    PRIVATE
    libs_utility)
add_to_libs_folder(libs_rendering)

unset(SOURCES)
unset(HEADERS)