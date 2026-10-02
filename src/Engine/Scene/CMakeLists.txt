# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    SceneSystem.cpp)
    
set(HEADERS
    SceneSystem.h)

add_library(engine_scene ${SOURCES} ${HEADERS})
target_link_libraries(engine_scene
    PUBLIC
    libs_ecs
    PRIVATE
    libs_resource
    libs_rendering
    )
set_property(TARGET engine_scene PROPERTY FOLDER "tactics/engine")
add_to_engine_folder(engine_scene)

set(TEST_SOURCES Tests/Test_SystemSubscriptions.cpp)
set(TEST_LIBS engine_scene libs_physics libs_resource_headless)
add_unit_test(engine_scene "${TEST_SOURCES}" "${TEST_LIBS}")

unset(TEST_SOURCES)
unset(TEST_LIBS)

unset(SOURCES)
unset(HEADERS)
