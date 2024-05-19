# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# Add all the subdirectory for the engine part
add_subdirectory(Rendering)
add_subdirectory(Resource)
add_subdirectory(Application)
