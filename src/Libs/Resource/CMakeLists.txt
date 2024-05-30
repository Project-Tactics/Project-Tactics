# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    IniFile/IniFile.cpp
    IniFile/IniFileLoader.cpp
    Resource.cpp
    ResourceLoader.cpp
    ResourceManager.cpp
    ResourceSystem.cpp
    ResourcePack/ResourcePackManager.cpp
    ResourcePack/ResourcePack.cpp)
    
set(HEADERS
    IniFile/IniFile.h
    IniFile/IniFileLoader.h
    Resource.h
    ResourceLoader.h
    ResourceManager.h
    ResourceProvider.h
    ResourceSystem.h
    ResourcePack/ResourcePackManager.h
    ResourcePack/ResourcePack.h)

add_library(libs_resource STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_resource
    PUBLIC
    inicpp
    json_wrapper
    libs_filesystem
    libs_utility)
add_to_libs_folder(libs_resource)

set(TEST_SOURCES 
    Tests/Test_ResourcePackDefinition.cpp
    Tests/Test_ResourcePack.cpp
    Tests/Test_ResourceSystemCommon.cpp
    Tests/Test_Fixture.h)
set(TEST_LIBS libs_resource)
add_unit_test(libs_resource "${TEST_SOURCES}" "${TEST_LIBS}")

unset(SOURCES)
unset(HEADERS)
unset(TEST_SOURCES)