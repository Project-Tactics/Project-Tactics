# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# Adding googletest as a dependency without installing it
set(INSTALL_GTEST OFF)
add_subdirectory(googletest)

# Adding glad as Loader-Generator for opengl
add_subdirectory(glad)