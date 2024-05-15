# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        UnitTestMain.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(ProjectTactics_UnitTests ${SOURCES})
target_link_libraries(ProjectTactics_UnitTests PUBLIC GTest::gtest GTest::gmock)

unset(SOURCES)