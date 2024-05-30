# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# Set the project name
project("ProjectTactics")

# Set C++20 as the standard to be used
set(CMAKE_CXX_STANDARD 20)
# Generate an error if C++20 support is not available
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Avoid conflicts with <Windows.h> min-max functions
add_compile_definitions(NOMINMAX)

# Suppress some compiler warnings to reduce noise
add_compile_definitions(_CRT_NONSTDC_NO_DEPRECATE)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

# Treat warnings as errors
add_compile_options(/WX)
# Set warning level 4
add_compile_options(/W4)
# Enable multi-processor compilation to accelerate the build process
add_compile_options(/MP)
# Enable a more standardized preprocessor behavior
add_compile_options(/Zc:preprocessor)

# Linker Optimization: Remove unreferenced data & functions
add_link_options(/OPT:REF)
# Linker Optimization: Merge identical code sections when possible
add_link_options(/OPT:ICF)

# Include macros from the cmakes folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmakes)
include(core_dependencies)
include(unit_tests)
include(folder_utils)

# Include the root folder of the framework code as an additional include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Include the 'thirdparty' folder containing third-party libraries
add_subdirectory(thirdparty)
# Include the 'src' folder containing the framework code
add_subdirectory(src)
# Include the 'resources' folder containing the data for all the apps being built
add_subdirectory(resources)

# Call this macro from the cmakes folder to resolve common dependencies like SDL, OpenGL, OpenAL and so on
core_dependencies()

# Set ProjectTactics as the default startup project for the generated Visual Studio solution
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ProjectTactics)
# Set a custom folder for the predefined targets in the generated Visual Studio solution
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_cmakeAndPostBuild")