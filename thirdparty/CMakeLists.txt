# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# Adding glad as Loader-Generator for opengl
# https://github.com/Dav1dde/glad
add_subdirectory(glad)

# Adding glm to work with OGL math
# https://github.com/g-truc/glm
add_subdirectory(glm)

# Adding imgui as immediate mode UI
# https://github.com/ocornut/imgui/tree/docking
add_subdirectory(imgui)
# https://github.com/thedmd/imgui-node-editor
add_subdirectory(imgui-node-editor)

# Adding fmt lib preferred to std::format for its runtime format string capabilities ( waiting for c++23 to get rid of it? )
# https://github.com/fmtlib/fmt
add_subdirectory(fmt)

# Adding stb library by only including std_image.h to load images
# https://github.com/nothings/stb
add_subdirectory(stb)

# Adding cereal as a serialization library
# https://github.com/USCiLab/cereal
add_subdirectory(cereal)

# Adding magic_enum for enum reflection
# https://github.com/Neargye/magic_enum
add_subdirectory(magic_enum)

# Adding inifile-cpp for reading .ini files
# https://github.com/Rookfighter/inifile-cpp
# We're not really in need of going through their cmakelists.txt
add_library(inicpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inifile-cpp/include/inicpp.h)
target_include_directories(inicpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inifile-cpp/include)

# Adding spdlog as a logging library
# https://github.com/gabime/spdlog
set(SPDLOG_FMT_EXTERNAL ON)
set(SPDLOG_WCHAR_SUPPORT ON)
add_subdirectory(spdlog)
target_compile_options(spdlog PRIVATE /wd4389)

# Adding entt as an entity-component-system library
# https://github.com/skypjack/entt
# We're not really in need of going through their cmakelists.txt
add_library(entt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entt/single_include/entt/entt.hpp)
target_include_directories(entt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entt/single_include)

# Let's keep things clean and tidy by moving all the thirdparty libs to a folder
set_property(TARGET glad glm imgui imgui_node_editor fmt stb inicpp entt cereal spdlog PROPERTY FOLDER "thirdparty")
# Let's move these extra CI targets that we don't need to the same thirdparty folder but later we'll just get rid of them
set_property(TARGET Continuous Experimental Nightly NightlyMemoryCheck PROPERTY FOLDER "thirdparty")