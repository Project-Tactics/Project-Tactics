# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(UNIT_TEST_MAIN_SOURCES
        UnitTestMain.cpp)

add_executable(ProjectTactics_UnitTests ${UNIT_TEST_MAIN_SOURCES})
target_link_libraries(ProjectTactics_UnitTests PUBLIC GTest::gtest GTest::gmock)

add_custom_target(UnitTest ProjectTactics_UnitTests
        DEPENDS ProjectTactics_UnitTests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        USES_TERMINAL)
