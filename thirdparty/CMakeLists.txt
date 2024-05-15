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

# Use luajit as a just in time for lua ( we're using a forked version with cmake support for now )
# https://github.com/WohlSoft/LuaJIT
add_subdirectory(luajit)
target_include_directories(libluajit INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/luajit/luajit/src)
set_target_properties(libluajit PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS}" LINK_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")

# Adding sol2 as template library to easily bind c++ to lua
# https://github.com/ThePhD/sol2
add_subdirectory(sol2)

# Let's keep things clean and tidy by moving all the thirdparty libs to a folder
set_property(TARGET gtest gtest_main gmock gmock_main glad glm imgui fmt sol2 PROPERTY FOLDER "thirdparty")
set_property(TARGET buildvm buildvm_arch_h minilua libluajit lj_gen_folddef lj_gen_headers lj_gen_vm_s luajit PROPERTY FOLDER "thirdparty/luajit")