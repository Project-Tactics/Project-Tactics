# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

add_subdirectory(Utility)
add_subdirectory(FileSystem)
add_subdirectory(Event)
add_subdirectory(Input)
add_subdirectory(Rendering)
add_subdirectory(Resource)
add_subdirectory(Scripting)
add_subdirectory(Overlay)
add_subdirectory(Fsm)
add_subdirectory(Ecs)
add_subdirectory(Ui)
