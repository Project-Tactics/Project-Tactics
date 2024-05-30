# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        ScriptingHelper.cpp)

set(HEADERS
        ScriptingHelper.h)

add_library(libs_scripting STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_scripting PUBLIC lua_wrapper PRIVATE libs_utility)
add_to_libs_folder(libs_scripting)

unset(SOURCES)
unset(HEADERS)