# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        Click/Backend/ClickSDLBackend.cpp
        Click/Click.cpp
        Click/ClickModifiers.cpp
        Click/ClickConditions.cpp
        InputSerialization.cpp
        InputSystem.cpp)

set(HEADERS
        Click/Backend/ClickSDLBackend.h
        Click/Click.h
        Click/ClickEnumTypes.h
        Click/ClickEvents.h
        Click/ClickInputTypes.h
        Click/ClickModifiers.h
        Click/ClickConditions.h
        InputSerialization.h
        InputSystem.h)

add_library(libs_input STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_input PRIVATE libs_utility sdl2_wrapper)
add_to_libs_folder(libs_input)

set(TEST_SOURCES Tests/Test_InputSystem.cpp)
set(TEST_LIBS libs_input libs_resource_headless sdl2_wrapper)
add_unit_test(libs_input "${TEST_SOURCES}" "${TEST_LIBS}")

unset(TEST_SOURCES)
unset(TEST_LIBS)
unset(SOURCES)
unset(HEADERS)
