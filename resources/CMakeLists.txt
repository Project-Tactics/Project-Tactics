# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

file(GLOB_RECURSE FILES *.*)

add_library(framework_data INTERFACE ${FILES})
add_to_data_folder(framework_data)

unset(FILES)