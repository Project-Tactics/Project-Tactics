# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# Add all the subdirectory for the engine part
add_subdirectory(Rendering)
add_subdirectory(Resource)
add_subdirectory(ECS)
add_subdirectory(Core)
add_subdirectory(Overlay)

add_library(tactics_engine INTERFACE)
target_link_libraries(tactics_engine INTERFACE engine_core engine_ecs engine_rendering engine_resource sdl2_wrapper imgui)