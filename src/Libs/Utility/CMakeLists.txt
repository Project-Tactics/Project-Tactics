# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(HEADERS
    ClassId.h
    Color.h
    Exception.h
    HashId.h
    ImGuiUtilities.h
    Ini/IniMathConverter.h
    Json/Json.h
    Json/JsonSerialization.h
    Log/Log.h
    Math.h
    Random.h
    Reflection.h
    Service/ServiceLocator.h
    String/Math.h
    String/String.h
    TransparentTypes.h
    Time/EngineTime.h
    Time/FrameTimer.h
    Time/TimeUtility.h
    Uniforms/UniformsDescriptor.h
    UUID.h)

set(SOURCES
    Color.cpp
    Exception.cpp
    HashId.cpp
    ImGuiUtilities.cpp
    Ini/IniMathConverter.cpp
    Json/Json.cpp
    Json/JsonSerialization.cpp
    Log/Log.cpp
    Math.cpp
    Random.cpp
    String/String.cpp
    Time/EngineTime.cpp
    Time/FrameTimer.cpp
    Time/TimeUtility.cpp
    Uniforms/UniformsDescriptor.cpp
    UUID.cpp)

add_library(libs_utility STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_utility PUBLIC glm 
    cpptrace::cpptrace fmt::fmt inicpp json_wrapper imgui entt magic_enum PRIVATE spdlog)
add_to_libs_folder(libs_utility)

set(TEST_SOURCES
    Tests/Test_FrameTimer.cpp
    Tests/Test_ServiceLocator.cpp
    Tests/Test_String.cpp)
set(TEST_LIBS libs_utility)
add_unit_test(libs_utility "${TEST_SOURCES}" "${TEST_LIBS}")
