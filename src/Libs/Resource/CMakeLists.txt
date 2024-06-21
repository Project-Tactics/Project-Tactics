# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    IniFile/IniFile.cpp
    IniFile/IniFileLoader.cpp
    Material/Material.cpp
    Material/MaterialLoader.cpp
    Material/Uniforms.cpp
    Mesh/Mesh.cpp
    Mesh/MeshLoader.cpp
    Prefab/Prefab.cpp
    Prefab/PrefabLoader.cpp
    Resource.cpp
    ResourceLoader.cpp
    ResourceManager.cpp
    ResourcePack/ResourcePackManager.cpp
    ResourcePack/ResourcePack.cpp
    ResourceSystem.cpp
    Shader/Shader.cpp
    Shader/ShaderLoader.cpp
    SpriteSheet/SpriteSheet.cpp
    SpriteSheet/SpriteSheetLoader.cpp
    Texture/Texture.cpp
    Texture/TextureLoader.cpp)
    
set(HEADERS
    IniFile/IniFile.h
    IniFile/IniFileLoader.cpp
    Material/Material.h
    Material/MaterialLoader.h
    Material/Uniforms.h
    Mesh/Mesh.h
    Mesh/MeshLoader.h
    Prefab/Prefab.h
    Prefab/PrefabLoader.h
    Resource.h
    ResourceLoader.h
    ResourceManager.h
    ResourcePack/ResourcePackManager.h
    ResourcePack/ResourcePack.h
    ResourceProvider.h
    ResourceSystem.h
    SpriteSheet/SpriteSheet.h
    SpriteSheet/SpriteSheetLoader.h
    Shader/Shader.h
    Shader/ShaderLoader.h
    Texture/Texture.h
    Texture/TextureLoader.h)

set(PUBLIC_LIBS
    libs_filesystem
    libs_utility
    libs_ecs
    inicpp
    json_wrapper
    stb
    cereal
    assimp_wrapper)

add_library(libs_resource STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_resource PUBLIC ${PUBLIC_LIBS} libs_rendering)
add_to_libs_folder(libs_resource)

add_library(libs_resource_headless STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_resource_headless PUBLIC ${PUBLIC_LIBS} libs_rendering_headless)
add_to_libs_folder(libs_resource_headless)

set(TEST_SOURCES 
    Tests/Test_Fixture.h
    Tests/Test_MaterialLoader.cpp
    Tests/Test_ResourcePackDefinition.cpp
    Tests/Test_ResourcePack.cpp
    Tests/Test_ResourceSystemCommon.cpp)
set(TEST_LIBS libs_resource_headless)
add_unit_test(libs_resource "${TEST_SOURCES}" "${TEST_LIBS}")

unset(SOURCES)
unset(HEADERS)
unset(TEST_SOURCES)