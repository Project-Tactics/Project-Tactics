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
# Enable multi-processor compilation to accelerate the build process
add_compile_options(/MP)
# Enable a more standardized preprocessor behavior
add_compile_options(/Zc:preprocessor)

# Linker Optimization: Remove unreferenced data & functions
add_link_options(/OPT:REF)
# Linker Optimization: Merge identical code sections when possible
add_link_options(/OPT:ICF)

# Include the 'src' folder containing the framework code
add_subdirectory(src)
# Include the 'thirdparty' folder containing third-party libraries - TODO(Gerark) Temporarily disabled
# add_subdirectory(thirdparty)

# Set ProjectTactics as the default startup project for the generated Visual Studio solution
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ProjectTactics)