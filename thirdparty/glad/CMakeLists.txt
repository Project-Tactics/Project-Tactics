# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(GLAD_SOURCES src/gl.c)

set(GLAD_HEADERS include/glad/gl.h
                    include/KHR/khrplatform.h)

add_library(glad STATIC ${GLAD_SOURCES} ${GLAD_HEADERS})
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
