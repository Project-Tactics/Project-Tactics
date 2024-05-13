# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(LIB_TESTLIB_HEADERS
        TestLibFile.h)

add_library(lib_testlib INTERFACE ${LIB_TESTLIB_HEADERS})

# Setup Test Project

set(TEST_LIB_TESTLIB_SOURCES
        Tests/Test_Example.cpp)

add_library(lib_testlib_unittest OBJECT ${TEST_LIB_TESTLIB_SOURCES})
target_link_libraries(lib_testlib_unittest PUBLIC lib_testlib GTest::gtest GTest::gmock)

target_link_libraries(ProjectTactics_UnitTests PUBLIC lib_testlib_unittest)