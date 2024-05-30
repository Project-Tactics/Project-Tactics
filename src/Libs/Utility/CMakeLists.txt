# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(HEADERS
    ClassId.h
    Color.h
    Exception.h
    Math.h
    TransparentTypes.h
    UUID.h
    Json/MathJsonSerializer.h
    Service/ServiceLocator.h)

set(SOURCES
    Color.cpp
    Exception.cpp
    Math.cpp
    Json/MathJsonSerializer.cpp
    UUID.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(libs_utility STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_utility PUBLIC glm fmt::fmt json_wrapper)
add_to_libs_folder(libs_utility)

set(TEST_SOURCES Tests/Test_ServiceLocator.cpp)
set(TEST_LIBS libs_utility)
add_unit_test(libs_utility ${TEST_SOURCES} ${TEST_LIBS})