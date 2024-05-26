# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# Adding googletest as a dependency without installing it
# https://github.com/google/googletest
set(INSTALL_GTEST OFF)
add_subdirectory(googletest)

# Adding glad as Loader-Generator for opengl
# https://github.com/Dav1dde/glad
add_subdirectory(glad)

# Adding glm to work with OGL math
# https://github.com/g-truc/glm
add_subdirectory(glm)

# Adding imgui as immediate mode UI
# https://github.com/ocornut/imgui/tree/docking
add_subdirectory(imgui)

# Adding fmt lib preferred to std::format for its runtime format string capabilities ( waiting for c++23 to get rid of it? )
# https://github.com/fmtlib/fmt
add_subdirectory(fmt)

# Adding sol2 as template library to easily bind c++ to lua
# https://github.com/ThePhD/sol2
add_subdirectory(sol2)

# Adding stb library by only including std_image.h to load images
# https://github.com/nothings/stb
add_subdirectory(stb)

# Adding inifile-cpp for reading .ini files
# https://github.com/Rookfighter/inifile-cpp
# We're not really in need of going through their cmakelists.txt
add_library(inicpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inifile-cpp/include/inicpp.h)
target_include_directories(inicpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inifile-cpp/include)

# Adding entt as an entity-component-system library
# https://github.com/skypjack/entt
# We're not really in need of going through their cmakelists.txt
add_library(entt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entt/single_include/entt/entt.hpp)
target_include_directories(entt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entt/single_include)

# Adding nlohmann/json for json parsing
# https://github.com/nlohmann/json
# We're not interested in the library unit tests
set(JSON_BuildTests OFF CACHE INTERNAL "")

# Let's keep things clean and tidy by moving all the thirdparty libs to a folder
set_property(TARGET gtest gtest_main gmock gmock_main glad glm imgui fmt sol2 stb inicpp entt PROPERTY FOLDER "thirdparty")