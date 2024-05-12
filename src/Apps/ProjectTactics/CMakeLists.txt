# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(APP_PROJECTTACTICS_SOURCES
        ProjectTacticsMain.cpp)

add_executable(ProjectTactics ${APP_PROJECTTACTICS_SOURCES})
