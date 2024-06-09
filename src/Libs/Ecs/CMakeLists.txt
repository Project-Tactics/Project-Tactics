cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Component/CameraComponent.cpp
    Component/ComponentReflection.cpp
    Component/FrustumComponent.cpp
    Component/MeshComponent.cpp
    Component/PrefabComponent.cpp
    Component/SpriteComponent.cpp
    Component/TransformComponent.cpp
    Entity.cpp
    EntityComponentSystem.cpp
    EntityUtilities.cpp
    PrefabManager.cpp
    System/CameraSystem.cpp
    System/SpriteSystem.cpp
    System/TransformSystem.cpp)
    
set(HEADERS
    Component/AlphaBlendedComponent.h
    Component/CameraComponent.h
    Component/ComponentReflection.h
    Component/FrustumComponent.h
    Component/MeshComponent.h
    Component/NameComponent.h
    Component/PrefabComponent.h
    Component/SpriteComponent.h 
    Component/TransformComponent.h
    Component/ViewportComponent.h
    Entity.h
    Entity.inl
    EntityComponentSystem.h
    EntityUtilities.h
    PrefabManager.h
    System/CameraSystem.h
    System/SpriteSystem.h
    System/TransformSystem.h)

add_library(libs_ecs STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_ecs PUBLIC entt glad libs_utility)
add_to_libs_folder(libs_ecs)

set(TEST_SOURCES Tests/Test_EntityComponentSystem.cpp)
set(TEST_LIBS libs_ecs)
add_unit_test(libs_ecs ${TEST_SOURCES} ${TEST_LIBS})

unset(SOURCES)
unset(HEADERS)
