# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

file(GLOB_RECURSE FILES data/*.*)

add_library(open_tactica_data INTERFACE ${FILES})
add_to_data_folder(open_tactica_data)

unset(FILES)