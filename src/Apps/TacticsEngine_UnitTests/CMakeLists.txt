# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        UnitTestMain.cpp)

set(TARGET_NAME TacticsEngine_UnitTests)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} PUBLIC GTest::gtest GTest::gmock)
target_compile_definitions(${TARGET_NAME} PUBLIC COMPILING_TESTS)

add_to_apps_folder(${TARGET_NAME})

unset(SOURCES)

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})