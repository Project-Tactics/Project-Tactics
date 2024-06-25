# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(HEADERS
    ClassId.h
    Color.h
    Exception.h
    Json/MathJsonSerializer.h
    Json/JsonSerialization.h
    Math.h
    Service/ServiceLocator.h
    HashString.h
    ImGuiUtilities.h
    Ini/IniMathConverter.h
    Reflection.h
    String.h
    TransparentTypes.h
    Uniforms/UniformsDescriptor.h
    UUID.h)

set(SOURCES
    Color.cpp
    Exception.cpp
    Math.cpp
    HashString.cpp
    ImGuiUtilities.cpp
    Ini/IniMathConverter.cpp
    Json/JsonSerialization.cpp
    Json/MathJsonSerializer.cpp
    String.cpp
    Uniforms/UniformsDescriptor.cpp
    UUID.cpp)

add_library(libs_utility STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_utility PUBLIC glm fmt::fmt inicpp json_wrapper imgui entt)
add_to_libs_folder(libs_utility)

set(TEST_SOURCES Tests/Test_ServiceLocator.cpp)
set(TEST_LIBS libs_utility)
add_unit_test(libs_utility ${TEST_SOURCES} ${TEST_LIBS})
