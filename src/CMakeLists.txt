# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# On GCC/Clang the warning flags are applied only to the framework code, so that thirdparty libraries
# (including the ones pulled with FetchContent) are not affected by them. MSVC flags are set in the root CMakeLists.txt
if(NOT MSVC)
    # Enable a high warning level and treat warnings as errors
    add_compile_options(-Wall -Wextra -Werror)
    # Partially initialized aggregates (e.g. {a, b} leaving the remaining members defaulted) are used on purpose
    add_compile_options(-Wno-missing-field-initializers)
endif()

add_subdirectory(Apps)
add_subdirectory(Libs)
add_subdirectory(Engine)
