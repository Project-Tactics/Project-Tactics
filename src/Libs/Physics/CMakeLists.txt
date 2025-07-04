# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        PhysicsSystem.cpp
        PhysicsSystemPimpl.cpp)

set(HEADERS
        JoltConstants.h
        JoltTraceAssert.h
        PhysicsSystem.h
        PhysicsSystemPimpl.h)

add_library(libs_physics STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_physics PRIVATE libs_ecs libs_utility Jolt::Jolt)
add_to_libs_folder(libs_physics)

unset(SOURCES)
unset(HEADERS)